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
			using chunk_node_t = typename chunk_list_t::node_t;

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

			static const constexpr size_t itemSize = o1::max(sizeof(item), sizeof(item::allocated) + sizeof(T));
			static const constexpr size_t alignment = sizeof(void*);

			static const constexpr size_t alignedItemSize =
				(itemSize & (alignment-1)) ?
				(itemSize | (alignment-1) + 1) :
				itemSize;

			static const constexpr size_t chunkDataSize = 16384; // TODO make this per-class tunable
			static const constexpr size_t maxEmptyChunks = 4; // TODO make this per-class tunable
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

			/**
			 * List of chunks w/ no busy items.
			 * @return
			 */
			static chunk_list_t& emptyChunks() {
				static chunk_list_t _chunks{getChunksNode};
				return _chunks;
			}

			/**
			 * List of chunks w/ busy and idle items.
			 * @return
			 */
			static chunk_list_t& nonFullChunks() {
				static chunk_list_t _chunks{getChunksNode};
				return _chunks;
			}

			/**
			 * List of full chunks.
			 * @return
			 */
			static chunk_list_t& fullChunks() {
				static chunk_list_t _chunks{getChunksNode};
				return _chunks;
			}

			static const alloc_metrics::pool_info* allocMetricsPoolInfo() {
				static alloc_metrics::pool_info metrics_pool_info{
					.items_per_chunk = itemsPerChunk,
					.bytes_per_chunk = bytesPerChunk,
					.item_size = sizeof(item),
				};
				return &metrics_pool_info;
			}

		public:

			chunk():
				objects(allocator().alloc()) {

				for (size_t i = 0; i < itemsPerChunk; ++i) {
					auto item = getItem(i);
					new (&item->idle._node) item_node_t();
					idleItems.push_back(&item->idle._node);
				}

				emptyChunks().push_back(this);
			}

			chunk(const chunk& that) = delete;

			chunk(chunk&& that) = delete;

			~chunk() {
				o1::xassert(idleItems.size() == itemsPerChunk, "Deleting a non-empty chunk!");
				allocator().free(objects);
			}

			void* alloc() {
				o1::xassert(!idleItems.empty(), "Requested chunk::alloc() on an full chunk");

				auto node = idleItems.pop_front();

				if (idleItems.empty()) {
					chunks_node.detach();
					fullChunks().push_back(this);

				} else if (idleItems.size() == itemsPerChunk - 1) {
					// If this was in the idleChunks() list.
					chunks_node.detach();
					nonFullChunks().push_back(this);

				}

				auto _item = (item*)(node);
				_item->allocated._chunk = this;
				return &_item->allocated._data[0];
			}

			void dealloc(void* p, alloc_metrics* _metrics) {
				o1::xassert(idleItems.size() != itemsPerChunk, "Requested chunk::alloc() on an empty chunk");

				auto _item = itemFromPointer(p);

				o1::xassert(
					_item->allocated._chunk == this,
					"chunk<T>::free() called with the wrong pointer!"
				);

				new (&_item->idle._node) item_node_t();

				idleItems.push_back(&_item->idle._node);

				bool chunk_freed = false;
				bool chunk_idled = false;

				if (idleItems.size() == itemsPerChunk) {
					chunk_idled = true;
					// move ourselves to the back
					chunks_node.detach();
					emptyChunks().push_back(this);
					if (emptyChunks().size() > maxEmptyChunks) {
						chunk_freed = true;
						delete emptyChunks().pop_front();
					}

				} else if (idleItems.size() == 1) {
					chunks_node.detach();
					nonFullChunks().push_back(this);

				}

				_metrics->deallocated(chunk_freed, chunk_idled, allocMetricsPoolInfo());
			}

			static void* allocate(alloc_metrics* _metrics) {
				auto& _nonFullChunks = nonFullChunks();

				if (!_nonFullChunks.empty()) {
					_metrics->allocated(false, false, allocMetricsPoolInfo());
					return _nonFullChunks.start()->ref()->alloc();
				}

				auto& _emptyChunks = emptyChunks();

				if (_emptyChunks.empty()) {
					_metrics->allocated(true, false, allocMetricsPoolInfo());
					auto _chunk = new chunk_t();
					return _chunk->alloc();
				}

				_metrics->allocated(false, true, allocMetricsPoolInfo());
				return _emptyChunks.start()->ref()->alloc();
			}

			static void free(void* p, alloc_metrics* _metrics) {
				auto item = itemFromPointer(p);
				item->allocated._chunk->dealloc(p, _metrics);
			}

			// TODO could gather statistics only when needed.

		};

	}

}

#endif //O1CPPLIB_O1_MEMORY_POOL_CHUNK_HH
