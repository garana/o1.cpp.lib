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


#ifndef O1CPPLIB_O1_HASH_OPS_T_HH
#define O1CPPLIB_O1_HASH_OPS_T_HH

#include <cstdint>
#include <cstddef>
#include "../list/o1.d_linked.list_t.hh"

namespace o1 {

	namespace hash {

		using hash_val = uint32_t;
		template <typename Value>
		using list_t = o1::d_linked::list_t<Value>;

		template <typename Value>
		using node = typename list_t<Value>::node;

		hash_val hashValue(const void* buf, size_t length, hash_val previousValue = 0);

		template <typename Key, typename Value>
		struct ops {
			typedef hash_val hashFn(const Key&);
			hashFn* hashValue;

			typedef const Key getKeyFn(const Value* value);
			getKeyFn* getKey;

			typedef typename o1::hash::list_t<Value>::node* getNodeFn(Value* value);
			getNodeFn* getNode;

			typedef bool equalFn(const Key& left, const Key& right);
			equalFn* equal;

		};

	}

}

#endif //O1CPPLIB_O1_HASH_OPS_T_HH
