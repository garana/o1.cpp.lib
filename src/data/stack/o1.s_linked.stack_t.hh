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

#ifndef O1CPPLIB_O1_S_LINKED_STACK_T_HH
#define O1CPPLIB_O1_S_LINKED_STACK_T_HH

#include "./o1.s_linked.stack.hh"
#include "../node/o1.s_linked.node_t.hh"

namespace o1 {

	namespace s_linked {

		template<typename T>
		class stack_t : protected o1::s_linked::stack {

		public:
			using node = o1::s_linked::node_t<T>;
			using getNodeFn = typename o1::s_linked::node_t<T>::getNodeFn;

		private:
			getNodeFn getNode{nullptr};

		public:

			stack_t() = delete;

			explicit stack_t(getNodeFn _getNode) : getNode(_getNode) {}

			stack_t(const stack_t& that) = delete;

			stack_t(stack_t&& that) noexcept:
				s_linked::stack(std::move(that)),
				getNode(that.getNode) {
				that.getNode = nullptr;
			}

			~stack_t() = default;

			using s_linked::stack::empty;

			/**
			 * "Alias" of stack::push(node).
			 * Push the @param node to the end of the stack.
			 * @param node element to be pushed.
			 */
			void push(T* node) {
				s_linked::stack::push(getNode(node));
			}

			/**
			 * "Alias" of stack::pop().
			 * Removes the element from the head of the stack.
			 * @return the element removed from the head stack, or nullptr if it's empty.
			 */
			T* pop() {
				return stack_t::node::ref(
					dynamic_cast<stack_t::node*>(
						s_linked::stack::pop()
					)
				);
			}

			/**
			 * Returns the element at the head of the stack.
			 * @return the element at the head of the stack, or nullptr if it's empty.
			 */
			T* peek() {
				return stack_t::node::ref(
					dynamic_cast<stack_t::node*>(
						s_linked::stack::peek()
					)
				);
			}

			/**
			 * Returns the element at the tail of the stack.
			 * @return the element at the tail of the stack, or nullptr if it's empty.
			 */
			const T* peek() const {
				return stack_t::node::ref(
					dynamic_cast<const stack_t::node*>(
						const_cast<const s_linked::stack::node*>(
							s_linked::stack::peek()
						)
					)
				);
			}
		};
	}

}

#endif //O1CPPLIB_O1_S_LINKED_STACK_T_HH
