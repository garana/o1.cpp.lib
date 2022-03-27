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


#ifndef O1CPPLIB_O1_HASH_BUCKET_T_HH
#define O1CPPLIB_O1_HASH_BUCKET_T_HH

#include "./o1.hash.ops_t.hh"

namespace o1 {

	namespace hash {

		template <
			typename Key,
			typename Value,
			struct ops<Key, Value>* ops
		>
		class buckets_t;

		template <
			typename Key,
			typename Value,
			struct ops<Key, Value>* ops
		>
		class bucket_t {
		public:

		private:

			static o1::d_linked::node_t<Value>*
			getBucketNode(Value* obj) {
				return ops->getNode(obj)->getBucketNode();
			}

			o1::hash::list_t<Value> nodes;

		protected:

			friend class buckets_t<Key,Value,ops>;

			Value* shift() {
				// TODO if node is not in the object => memleak, add an "embedded" boolean template param?
				return nodes.pop_front();
			}

		public:
			bucket_t(): nodes(getBucketNode) { };

			bucket_t(const bucket_t& that) = delete;

			bucket_t(bucket_t&& that) = delete;

			~bucket_t() = default;

			bool empty() { return nodes.empty(); }

			bool empty() const { return nodes.empty(); }

			bool insert(
				const Key& key,
				Value* value
			) {

				for (auto i: nodes) {
					if (ops->equal(key, ops->getKey(i)))
						return false;
				}

				nodes.push_back(value);
				return true;
			}

			/**
			 * Returns true if the entry was added and not replaced.
			 * @param value
			 * @param old_value
			 * @return
			 */
			bool set(
				const Key& key,
				Value* value,
				Value** old_value
			) {
				if (old_value != nullptr)
					*old_value = nullptr;

				bool replaced = false;

				for (auto i: nodes) {
					if (ops->equal(key, ops->getKey(*i))) {
						if (old_value != nullptr)
							*old_value = *i;
						ops->getNode(*i).detach(); // TODO "embedded" flag
						replaced = true;
						break;
					}
				}

				nodes.push_back(value);
				return !replaced;
			}


			bool replace(
				const Key& key,
				Value value,
				Value* old_value
			) {
				if (old_value != nullptr)
					*old_value = nullptr;

				for (auto i: nodes) {
					if (ops->equal(key, ops->getKey(*i))) {
						if (old_value != nullptr)
							*old_value = *i;
						ops->getNode(*i).detach(); // TODO "embedded" flag
						nodes.push_back(value);
						return true;
					}
				}

				return false;
			}

			bool remove(
				const Key& key,
				Value** old_value
			) {
				if (old_value != nullptr)
					*old_value = nullptr;

				for (auto i: nodes) {
					if (ops->equal(key, ops->getKey(i))) {
						if (old_value != nullptr)
							*old_value = i;
						getBucketNode(i)->detach(); // TODO "embedded" flag
						return true;
					}
				}

				return false;
			}

			Value* find(const Key& key) {
				for (auto i: nodes) {
					if (ops->equal(key, ops->getKey(i))) {
						return i;
					}
				}
				return nullptr;
			}

		};

	}

}

#endif //O1CPPLIB_O1_HASH_BUCKET_T_HH
