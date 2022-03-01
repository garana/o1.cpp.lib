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


#ifndef O1CPPLIB_O1_HASH_BUCKETS_T_HH
#define O1CPPLIB_O1_HASH_BUCKETS_T_HH

#include "../../o1.logging.hh"
#include "../list/o1.d_linked.list.hh"
#include "./o1.hash.bucket_t.hh"

namespace o1 {

	namespace hash {

		template <
			typename Key,
			typename Value,
			struct ops<Key, Value>* ops
		>
		class buckets_t {
			/**
			 * Number of buckets.
			 * Length of the buckets array.
			 */
			size_t bucketsCount;

			/**
			 * Array of buckets.
			 * Each position may be null.
			 */
			bucket_t<Key,Value,ops>** buckets{nullptr};

			/**
			 * Number of non-null buckets
			 */
			size_t nonNullBucketsCount{0};

		protected:

			typedef enum {
				gboNONE = 0,
				gboAlloc = 1,
				gboDeleteIfEmpty = 2
			} GetBucketOptions;

			void deleteIfEmpty(bucket_t<Key,Value,ops>** bucket) {
				if ((*bucket)->empty()) {
					delete *bucket;
					*bucket = nullptr;
					--nonNullBucketsCount;
				}
			}

			bucket_t<Key,Value,ops>* getBucket(
				hash_val hashValue,
				GetBucketOptions options = gboNONE
			) {
				if (buckets == nullptr) {
					if ((options & gboAlloc) == 0)
						return nullptr;
					buckets = new bucket_t<Key, Value, ops>* [bucketsCount]{nullptr};
					o1::xassert(nonNullBucketsCount == 0, "bucket_t::getBucket: internal inconsistency");
				}

				auto bucket = &buckets[hashValue % bucketsCount];

				if (*bucket == nullptr) {

					if (options & gboAlloc) {
						*bucket = new bucket_t<Key, Value, ops>();
						++nonNullBucketsCount;
					}

				} else if (options & gboDeleteIfEmpty) {
					deleteIfEmpty(bucket);
				}

				return *bucket;
			}

		public:
			buckets_t() = delete;

			explicit buckets_t(size_t numBuckets):
				bucketsCount(numBuckets) {
			}

			buckets_t(const buckets_t& that) = delete;

			buckets_t(buckets_t&& that) = delete;

			~buckets_t() {
				if (buckets == nullptr)
					return;

				size_t visited = 0;
				for (size_t i = 0; i < bucketsCount; ++i) {
					if (buckets[i] == nullptr)
						continue;
					++visited;

					delete buckets[i];

					if (visited == nonNullBucketsCount)
						break;
				}

				delete[] buckets;
			}

			bool empty() const { return nonNullBucketsCount == 0;}

			bool insert(
				const Key& key,
				hash_val hashValue,
				Value* value
			) {
				return getBucket(hashValue, gboAlloc)
					->insert(key, value);
			}

			/**
			 * Returns true if the entry was added and not replaced.
			 * @param key
			 * @param hashValue
			 * @param value
			 * @param old_value
			 * @return
			 */
			bool set(
				const Key& key,
				hash_val hashValue,
				Value* value,
				Value** old_value
			) {
				return getBucket(hashValue, gboAlloc)
					->set(key, value, old_value);
			}

			bool replace(
				const Key& key,
				hash_val hashValue,
				Value* value,
				Value** old_value
			) {
				auto bucket = getBucket(hashValue, gboDeleteIfEmpty);
				if (!bucket)
					return false;

				return bucket->replace(key, value, old_value);
			}

			bool remove(
				const Key& key,
				hash_val hashValue,
				Value** old_value
			) {
				auto bucket = getBucket(hashValue, gboDeleteIfEmpty);

				if (!bucket)
					return false;

				return bucket->remove(key, old_value);
			}

			Value* find(const Key& key, hash_val hashValue) {
				auto bucket = getBucket(hashValue, gboDeleteIfEmpty);

				if (!bucket)
					return nullptr;

				return bucket->find(key);
			}

			void rehashInto(buckets_t<Key,Value,ops>* that, hash_val hashValue) {
				auto bucket = getBucket(hashValue, gboDeleteIfEmpty);

				if (bucket == nullptr)
					return;

				while (auto _value = bucket->shift()) {
					auto _key = ops->getKey(_value);
					auto _hashValue = ops->hashValue(_key);
					auto inserted = that->insert(_key, _hashValue, _value);
					o1::xassert(inserted, "o1::hash::buckets_t::rehashInto found a duplicate entry");
				}

				delete bucket;
				buckets[hashValue % bucketsCount] = nullptr;
				--nonNullBucketsCount;
			}

		};

	}

}
#endif //O1CPPLIB_O1_HASH_BUCKETS_T_HH
