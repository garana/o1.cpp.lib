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

#ifndef O1CPPLIB_O1_STACK_T_HH
#define O1CPPLIB_O1_STACK_T_HH

#include "o1.stack.hh"
#include "../o1.member.hh"

namespace o1 {

	template <typename Node, std::ptrdiff_t node_offset>
	class stack_t: protected o1::stack {

	public:
		class node: protected o1::stack::node {
			friend class stack_t<Node, node_offset>;
		};

	protected:
		using member = o1::member<Node, stack_t::node, node_offset>;

	public:

		stack_t() = default;

		stack_t(const stack_t& that) = delete;

		stack_t(stack_t&& that) noexcept:
			stack(std::move(that)) {
		}

		~stack_t() = default;

		using stack::empty;

		/**
		 * "Alias" of stack::push(node).
		 * Push the @param node to the end of the stack.
		 * @param node element to be pushed.
		 */
		void push(Node* node) {
			stack::push_back(member::node2member(node));
		}

		/**
		 * "Alias" of stack::pop().
		 * Removes the element from the head of the stack.
		 * @return the element removed from the head stack, or nullptr if it's empty.
		 */
		Node* pop() {
			return member::member2node(static_cast<stack_t::node*>(stack::pop()));
		}

		/**
		 * Returns the element at the head of the stack.
		 * @return the element at the head of the stack, or nullptr if it's empty.
		 */
		Node* peek() {
			return member::member2node(static_cast<stack_t::node*>(stack::peek()));
		}

		/**
		 * Returns the element at the tail of the stack.
		 * @return the element at the tail of the stack, or nullptr if it's empty.
		 */
		const Node* peek() const {
			return member::member2node(static_cast<const stack_t::node*>(stack::peek()));
		}
	};
}

#endif //O1CPPLIB_O1_STACK_T_HH
