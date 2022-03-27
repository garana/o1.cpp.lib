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

#ifndef O1CPPLIB_O1_S_LINKED_QUEUE_T_HH
#define O1CPPLIB_O1_S_LINKED_QUEUE_T_HH

#include "./o1.s_linked.queue.hh"
#include "../node/o1.s_linked.node_t.hh"

namespace o1 {

	namespace s_linked {

		// TODO add doc
		template<typename T>
		class queue_t : protected o1::s_linked::queue {

		public:
			using node = o1::s_linked::node_t<T>;
			using getNodeFn = typename o1::s_linked::node_t<T>::getNodeFn;

		private:
			getNodeFn getNode{nullptr};

		public:

			queue_t() = delete;

			explicit queue_t(getNodeFn _getNode) : getNode(_getNode) {}

			queue_t(const queue_t& that) = delete;

			queue_t(queue_t&& that) noexcept:
				s_linked::queue(std::move(that)),
				getNode(that.getNode) {
				that.getNode = nullptr;
			}

			~queue_t() = default;

			using s_linked::queue::empty;
			using s_linked::queue::size;

			/**
			 * "Alias" of queue::push(node).
			 * Push the @param node to the end of the queue.
			 * @param node element to be pushed.
			 */
			void push(T* node) {
				s_linked::queue::push_back(getNode(node));
			}

			/**
			 * "Alias" of queue::pop().
			 * Removes the element from the head of the queue.
			 * @return the element removed from the head queue, or nullptr if it's empty.
			 */
			T* pop() {
				return queue_t::node::ref(
					dynamic_cast<queue_t::node*>(
						s_linked::queue::pop()
					)
				);
			}

			/**
			 * Returns the element at the head of the queue.
			 * @return the element at the head of the queue, or nullptr if it's empty.
			 */
			T* peek() {
				return queue_t::node::ref(
					dynamic_cast<queue_t::node*>(
						s_linked::queue::peek()
					)
				);
			}

			/**
			 * Returns the element at the tail of the queue.
			 * @return the element at the tail of the queue, or nullptr if it's empty.
			 */
			const T* peek() const {
				return queue_t::node::ref(
					dynamic_cast<const queue_t::node*>(
						const_cast<const s_linked::queue::node*>(
							s_linked::queue::peek()
						)
					)
				);
			}
		};
	}

}

#endif //O1CPPLIB_O1_S_LINKED_QUEUE_T_HH
