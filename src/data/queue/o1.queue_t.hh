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

#ifndef O1CPPLIB_O1_QUEUE_T_HH
#define O1CPPLIB_O1_QUEUE_T_HH

#include "o1.queue.hh"
#include "../o1.member.hh"

namespace o1 {

	template <typename Node, std::ptrdiff_t node_offset>
	class queue_t: protected o1::queue {

	public:
		class node: protected o1::queue::node {
			friend class queue_t<Node, node_offset>;
		};

	protected:
		using member = o1::member<Node, queue_t::node, node_offset>;

	public:

		queue_t() = default;

		queue_t(const queue_t& that) = delete;

		queue_t(queue_t&& that) noexcept:
			queue(std::move(that)) {
		}

		~queue_t() = default;

		using queue::empty;

		/**
		 * "Alias" of queue::push(node).
		 * Push the @param node to the end of the queue.
		 * @param node element to be pushed.
		 */
		void push(Node* node) {
			queue::push_back(member::node2member(node));
		}

		/**
		 * "Alias" of queue::pop().
		 * Removes the element from the head of the queue.
		 * @return the element removed from the head queue, or nullptr if it's empty.
		 */
		Node* pop() {
			return member::member2node(static_cast<queue_t::node*>(queue::pop()));
		}

		/**
		 * Returns the element at the head of the queue.
		 * @return the element at the head of the queue, or nullptr if it's empty.
		 */
		Node* peek() {
			return member::member2node(static_cast<queue_t::node*>(queue::peek()));
		}

		/**
		 * Returns the element at the tail of the queue.
		 * @return the element at the tail of the queue, or nullptr if it's empty.
		 */
		const Node* peek() const {
			return member::member2node(static_cast<const queue_t::node*>(queue::peek()));
		}
	};
}

#endif //O1CPPLIB_O1_QUEUE_T_HH
