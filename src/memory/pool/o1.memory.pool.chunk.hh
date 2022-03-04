/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Gonzalo Arana
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef O1CPPLIB_O1_MEMORY_POOL_CHUNK_HH
#define O1CPPLIB_O1_MEMORY_POOL_CHUNK_HH

#include <cstddef>
#include <vector>
#include "../../data/list/o1.d_linked.list_t.hh"
#include "../../data/list/o1.s_linked.list.hh"
#include "../allocator/o1.memory.mmap_allocator.hh"
#include "../../o1.logging.hh"
#include "../../o1.math.hh"

namespace o1 {

	namespace memory {

		template<typename T>
		class chunk {

		public:

			using chunk_t = chunk<T>;

			// List of chunks of T.
			// Empty chunks are kept on the back of the list.
			using chunk_list_t = o1::d_linked::list_t<chunk_t>;
			using chunk_node_t = typename chunk_list_t::node;

			using item_list_t = o1::s_linked::list;
			using item_node_t = typename o1::s_linked::list::node;

			union item {
				struct {
					chunk* _chunk;
					char _data[0];
				} allocated;

				struct {
					item_node_t _node;
				} idle;
			};

			static item* itemFromPointer(void* p) {
				return reinterpret_cast<item*>(
					(static_cast<void**>(p) - 1)
				);
			}

			static const constexpr size_t minSize = sizeof(item);
			static const constexpr size_t itemSize = o1::max(minSize, sizeof(T));
			static const constexpr size_t alignment = sizeof(void*);

			static const constexpr size_t alignedItemSize =
				(itemSize & (alignment-1)) ?
				(itemSize | (alignment-1) + 1) :
				itemSize;

			static const constexpr size_t chunkDataSize = 16384; // TODO make this per-class tunable
			static const constexpr size_t maxIdleChunks = 4; // TODO make this per-class tunable
			static const constexpr size_t bytesPerChunk = chunkDataSize + sizeof(chunk_t);

			static const constexpr size_t itemsPerChunk = chunkDataSize / alignedItemSize;

			static mmap_allocator& allocator() {
				static mmap_allocator _allocator{chunkDataSize};
				return _allocator;
			}

		private:

			void* objects;
			chunk_node_t chunks_node{this};
			item_list_t idleItems;
			static size_t _emptyChunksCount;
			static size_t _idleItemsCount;

			/**
			 * Given an index, return the item address.
			 */
			item* getItem(size_t index) {
				o1::xassert(index < itemsPerChunk, "index out of range");
				return reinterpret_cast<item*>(
					((uint8_t*)objects) + index * alignedItemSize
				);
			}

			/**
			 * Get the item index from it's address.
			 */
			size_t getIndex(item* item) {
				size_t byte_index = (uint8_t*)item - (uint8_t*)objects;
				o1::xassert(byte_index % alignedItemSize == 0, "getIndex with wrong pointer");
				return byte_index / alignedItemSize;
			}

			static chunk_node_t* getChunksNode(chunk_t* that) {
				return &that->chunks_node;
			}

			static chunk_list_t& chunks() {
				static chunk_list_t _chunks{getChunksNode};
				return _chunks;
			}

		public:

			chunk():
				objects(allocator().alloc()) {

				for (size_t i = 0; i < itemsPerChunk; ++i) {
					auto item = getItem(i);
					new (&item->idle._node) item_node_t();
					idleItems.push_back(&item->idle._node);
				}

				_idleItemsCount += itemsPerChunk;

				chunks().push_back(this);
				++_emptyChunksCount;
			}

			chunk(const chunk& that) = delete;

			chunk(chunk&& that) = delete;

			~chunk() {
				o1::xassert(empty(), "Deleting a non-empty chunk!");
				--_emptyChunksCount;
				_idleItemsCount -= itemsPerChunk;
				allocator().free(objects);
			}

			void* alloc() {
				if (empty()) {
					--_emptyChunksCount;
					chunks_node.detach();
					chunks().push_front(this);
				}

				auto node = idleItems.pop_front();
				--_idleItemsCount;

				o1::xassert(
					node != nullptr,
					"chunk<T>::alloc() called for a full chunk?"
				);

				auto _item = (item*)(node);
				_item->allocated._chunk = this;
				return &_item->allocated._data[0];
			}

			void dealloc(void* p) {
				auto _item = itemFromPointer(p);

				o1::xassert(
					_item->allocated._chunk == this,
					"chunk<T>::free() called with the wrong pointer!"
				);

				new (&_item->idle._node) item_node_t();

				idleItems.push_back(&_item->idle._node);
				++_idleItemsCount;

				if (empty()) {
					// move ourselves to the back
					++_emptyChunksCount;
					chunks_node.detach();
					if (_emptyChunksCount > maxIdleChunks)
						delete chunks().pop_back();
					chunks().push_back(this);
				}
			}

			static void* allocate() {
				auto& _chunks = chunks();
				auto _ichunk = chunks().rbegin();
				auto _chunk = _ichunk == chunks().rend() ?
					new chunk_t() :
					*_ichunk;
				return _chunk->alloc();
			}

			static void free(void* p) {
				auto item = itemFromPointer(p);
				item->allocated._chunk->dealloc(p);
			}

			bool empty() { return idleItems.size() == itemsPerChunk; }

			struct alloc_metric {
				size_t total;
				size_t idle;
			};

			struct alloc_metrics {
				alloc_metric items;
				alloc_metric chunks;
				alloc_metric bytes;
			};

			static void getAllocMetrics(alloc_metrics* metrics) {
				metrics->chunks.idle = _emptyChunksCount;
				metrics->chunks.total = chunks().size();

				metrics->items.idle = _idleItemsCount;
				metrics->items.total = metrics->chunks.total * itemsPerChunk;

				metrics->bytes.idle = _idleItemsCount * itemSize;
				metrics->bytes.total = metrics->chunks.total * bytesPerChunk;
			}

		};


		template <typename T>
		size_t chunk<T>::_emptyChunksCount = 0;

		template <typename T>
		size_t chunk<T>::_idleItemsCount = 0;

	}

}

#endif //O1CPPLIB_O1_MEMORY_POOL_CHUNK_HH
