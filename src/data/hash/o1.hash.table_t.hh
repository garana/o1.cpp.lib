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


#ifndef O1CPPLIB_O1_HASH_TABLE_T_HH
#define O1CPPLIB_O1_HASH_TABLE_T_HH

#include <cstdint>
#include "./o1.hash.ops_t.hh"
#include "./o1.hash.bucket_t.hh"
#include "./o1.hash.buckets_t.hh"
#include "./o1.hash.sizing_strategy.hh"
#include "./o1.hash.conf.hh"

namespace o1 {

	namespace hash {

		/**
		 * If the key is an integer, perhaps it's a good thing to convert it
		 * to network byte order (using o1::hton<int_type_t>) to make it
		 * behave consistently in different architectures.
		 * @tparam Key
		 * @tparam Value
		 * @tparam ops
		 */
		template <
		    typename Key,
			typename Value,
			struct ops<Key, Value>* ops
		>
		class table {
		protected:

			using buckets_t = o1::hash::buckets_t<Key, Value, ops>;

			sizing_strategy sizingStrategy;

			/**
			 * Number of elements.
			 * May not be accurate if a node gets detached on it's own.
			 * Gets updated on hash hash slot traversal.
			 */
			size_t numElements{0};

			/**
			 * Slots vector, an array of bucket vectors, each of different
			 * size.
			 * There is a currentSlot (based on the current number of elements),
			 * all others get (slowly) rehashed into the it.
			 */
			buckets_t** slots{nullptr};

			/**
			 * The active hash table bucket vector.
			 */
			size_t currentSlot{0};

			buckets_t* getCurrentSlot() {
				o1::xassert(slots[currentSlot] != nullptr,
					"forgot to allocate currentSlot!");
				return slots[currentSlot];
			}

			void rehash(hash_val hashValue) {

				currentSlot = sizingStrategy.sizeIndex(currentSlot, numElements);

				if (slots == nullptr)
					slots = new buckets_t*[sizingStrategy.maxSizingIndex() + 1]{nullptr};

				if (slots[currentSlot] == nullptr)
					slots[currentSlot] = new buckets_t(sizingStrategy.numBuckets(currentSlot));

				for (
					size_t iSlot = 0;
					iSlot <= sizingStrategy.maxSizingIndex();
					++iSlot)
				{
					if (iSlot == currentSlot)
						continue;

					if (slots[iSlot] == nullptr)
						continue;

					slots[iSlot]->rehashInto(slots[currentSlot], hashValue);

					if (slots[iSlot]->empty()) {
						delete slots[iSlot];
						slots[iSlot] = nullptr;
					}

				}

			}

		public:
			table():
				sizingStrategy() {
			}

			/**
			 * @param maxElements this is only a hint, to decide the maximum size
			 *                    of the bucket vector.
			 */
			explicit table(size_t maxElements):
				sizingStrategy(O1_HASH_TABLE_DEFAULT_LOAD_EXPONENT, maxElements) {
			}

			~table() {
				clear();
			}

			bool insert(Value* value) {
				auto key = ops->getKey(value);
				hash_val hashValue = ops->hashValue(key);
				rehash(hashValue);
				auto retVal = getCurrentSlot()->insert(key, hashValue, value);
				if (retVal)
					++numElements;
				return retVal;
			}

			/**
			 * Inserts or updates the key=ops->getKey(value) entry with the
			 * passed value.
			 * @param value
			 * @param old_value if !nullptr, existing value (if any) is stored here.
			 * @return true if the entry was not found and added.
			 */
			bool set(Value* value, Value** old_value = nullptr) {
				auto key = ops->getKey(value);
				hash_val hashValue = ops->hashValue(key);
				rehash(hashValue);
				auto retVal = getCurrentSlot()->set(key, hashValue, value, old_value);
				if (retVal)
					++numElements;
				return retVal;
			}

			/**
			 * Stores the passed value only if it was already present.
			 * @param value
			 * @param old_value
			 * @returntrue if the entry was found and replaced.
			 */
			bool replace(Value* value, Value** old_value = nullptr) {
				auto key = ops->getKey(value);
				hash_val hashValue = ops->hashValue(key);
				rehash(hashValue);
				return getCurrentSlot()->replace(key, hashValue, value, old_value);
			}

			/**
			 * Removes the entry with key=ops->getKey(value).
			 * @param value entry
			 * @param old_value if not a nullptr, existing value is stored here.
			 * @return
			 */
			bool remove(Value* value, Value** old_value = nullptr) {
				return remove(ops->getKey(value), old_value);
			}

			bool remove(const Key& key, Value** old_value = nullptr) {
				hash_val hashValue = ops->hashValue(key);
				rehash(hashValue);
				auto retVal = getCurrentSlot()->remove(key, hashValue, old_value);
				if (retVal)
					--numElements;
				return retVal;
			}

			Value* find(const Key& key) {
				hash_val hashValue = ops->hashValue(key);
				rehash(hashValue);
				return getCurrentSlot()->find(key, hashValue);
			}

			/**
			 * Remove all entries.
			 */
			void clear() {
				for (int i = 0; i <= sizingStrategy.maxSizingIndex(); ++i) {
					delete slots[i];
					slots[i] = nullptr;
				}
				delete[] slots;
				currentSlot = 0;
				numElements = 0;
			}

		};

	}

}

#endif //O1CPPLIB_O1_HASH_TABLE_T_HH
