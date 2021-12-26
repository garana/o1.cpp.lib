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

#ifndef O1CPPLIB_O1_MEMBER_HH
#define O1CPPLIB_O1_MEMBER_HH

#include <cstddef>
#include <type_traits>

namespace o1 {

	template <typename Node, typename Member, std::ptrdiff_t node_offset>
	class member {
	public:
		static_assert(std::is_standard_layout<Node>::value, "o1::member only works for std layout nodes");

		static Member* node2member(Node* pNode) {
			return pNode == nullptr ?
				   nullptr :
				   (Member*)( (char*) pNode + node_offset );
		}

		static const Member* node2member(const Node* pNode) {
			return pNode == nullptr ?
				   nullptr :
				   (const Member*)( (char*) pNode + node_offset );
		}

		static Node* member2node(Member* member) {
			return member == nullptr ?
				   nullptr :
				   (Node*)( (char*) member - node_offset );
		}

		static const Node* member2node(const Member* member) {
			return member == nullptr ?
				   nullptr :
				   (const Node*)( (char*) member - node_offset );
		}

	};
}

#endif //O1CPPLIB_O1_MEMBER_HH
