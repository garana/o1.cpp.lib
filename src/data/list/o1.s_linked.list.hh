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

#ifndef O1_LIB_SLL_HH
#define O1_LIB_SLL_HH

#include <cstddef>
#include <algorithm>
#include "../node/o1.s_linked.node.hh"

namespace o1 {

	namespace s_linked {

		/**
		 * Simply Linked List.
		 *
		 * If a node gets destroyed (it's destructor called) before the list itself does,
		 * the list will be inconsistent.  The preferred implementation is d_linked, which
		 * remains consistent in this case.
		 *
		 * These operations are supported (all O(1)):
		 * - push_back (push, append)
		 * - push_front (insert)
		 * - pop_front (shift)
		 *
		 * This data structure does not allow a random node to be detached.
		 *
		 */
		class list {
		public:

			using node = o1::s_linked::node;

		private:

			node _head;
			node* _tail;

		public:
			list() : _tail(&_head) {};

			list(const list& that) = delete;

			list(list&& that) noexcept;

			/**
			 * Upon destruction, entries are NOT deleted.
			 * Nodes with destructor called before the list is destroyed lead to a corrupt list.
			 */
			~list() = default;

			[[nodiscard]] const node* start() const { return _head.next(); }

			node* start() { return _head.next(); }

			/**
				 * @return true iff the list is empty.
				 */
			[[nodiscard]] bool empty() const {
				return _head.next() == nullptr;
			}

			// TODO add size() method, also to stack & queue

			/**
			 * @return true iff the list is empty.
			 */
			bool empty() { return _head.next() == nullptr; }

			/**
			 * Adds the @param node to the end of the list.
			 * @param node node inserted at the end of the list.
			 */
			void push_back(node* node);

			/**
			 * Inserts @param node at the beginning of the list.
			 * @param node node inserted at the beginning of the list.
			 */
			void push_front(node* node);

			/**
			 * Removes (and returns) the first node of the list.
			 * @return the node removed from the front of the list, nullptr if the list is empty.
			 */
			node* pop_front();

		};

	}

}

#endif //O1_LIB_SLL_HH
