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

#ifndef O1CPPLIB_O1_S_LINKED_STACK_HH
#define O1CPPLIB_O1_S_LINKED_STACK_HH

#include "../list/o1.s_linked.list.hh"

namespace o1 {

	namespace s_linked {

		/**
		 * TODO sync doc w/ list
		 * Non-Owning, simply linked stack.
		 * Nodes can't detach themselves from the stack.
		 */
		class stack : protected o1::s_linked::list {

		public:

			using node = o1::s_linked::node;

			stack() = default;

			stack(const stack& that) = delete;

			stack(stack&& that) noexcept:
				list(std::move(that)) {
			}


			/**
			 * Upon destruction, entries are NOT deleted.
			 * They'll form a  double linked list on their own.
			 */
			~stack() = default;

			using o1::s_linked::list::empty;

			/**
			 * Push the arg to the stack.
			 * @param node element to be pushed.
			 */
			inline void push(node* node) {
				list::push_front(node);
			}

			/**
			 * Removes the element from the top of the stack.
			 * @return the element removed from the stack, or nullptr if it's empty.
			 */
			inline node* pop() {
				return reinterpret_cast<node*>(list::pop_front());
			}

			/**
			 * Returns the element at the top of the stack.
			 * @return the element at the top of the stack, or nullptr if it's empty.
			 */
			inline node* peek() {
				return empty() ?
					   nullptr :
					   reinterpret_cast<node*>(start());
			}

			/**
			 * Returns the element at the top of the stack.
			 * @return the element at the top of the stack, or nullptr if it's empty.
			 */
			const node* peek() const {
				return empty() ?
					   nullptr :
					   reinterpret_cast<const node*>(start());
			}

		};

	}

}

#endif //O1CPPLIB_O1_S_LINKED_STACK_HH
