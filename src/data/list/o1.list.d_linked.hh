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

#ifndef O1_LIB_DLL_HH
#define O1_LIB_DLL_HH

#include <cstddef>
#include <algorithm>

namespace o1 {

	namespace list {

		/**
		 * Double (circular) Linked List.
		 *
		 * If a node gets destroyed (it's destructor called) before the list itself does,
		 * the list remains consistent, as each node detaches it self from the list.
		 *
		 * These operations are supported (all O(1)):
		 * - push_back (push, append)
		 * - push_front (insert)
		 * - pop_back (pop)
		 * - pop_front (shift)
		 * - Any random node detached.
		 *
		 * Instead of keeping a list of nodes with a pointer to the data, the data structure
		 * should include the d_linked::node.
		 *
		 * Note that the d_linked::node next() and prev() would point to the node member, not the
		 * node data structure.
		 */
		class d_linked {
			
		public:
			
			class node {
				node* _next = nullptr;
				node* _prev = nullptr;

			public:
				node() {
					_prev = _next = this;
				}

				node(const node& that) = delete;

				/**
				 * Move constructor.
				 * Replace @param that with *this on the list.
				 * @param that
				 */
				node(node&& that) noexcept;

				/**
				 * Detach this node on destructor.
				 */
				~node() { detach(); }

				void detach();

				[[nodiscard]] const node* next() const { return _next; }

				node* next() { return _next; }

				[[nodiscard]] const node* prev() const { return _prev; }

				node* prev() { return _prev; }

				/**
				 * Inserts @param next after *this.
				 */
				void push_back(node* next);

				/**
				 * Inserts @param next before *this.
				 */
				void push_front(node* next);

				/**
				 * @return if this node is the only one in the list.
				 */
				bool empty();

				/**
				 * @return if this node is the only one in the list.
				 */
				bool empty() const;

			};

		private:

			node _node;

		public:
			d_linked() = default;

			d_linked(const d_linked& that) = delete;

			d_linked(d_linked&& that) noexcept:
				_node(std::move(that._node)) {
			}

			/**
			 * Upon destruction, entries are NOT deleted.
			 * They'll form a  double linked list on their own.
			 */
			~d_linked() = default;

			/**
			 * First node of the list.
			 * @return the first node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] const node* start() const { return _node.next(); }

			/**
			 * First node of the list.
			 * @return the first node of the list, or finish() if it's empty.
			 */
			node* start() { return _node.next(); }

			/**
			 * Last element of the list.
			 * @return the last node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] const node* r_start() const { return _node.prev(); }

			/**
			 * Last element of the list.
			 * @return the last node of the list, or finish() if it's empty.
			 */
			node* r_start() { return _node.prev(); }

			/**
			 * When a node equals to the returned value, list traversal has ended.
			 * @return a node that's not in the list, is the node of the list itself.
			 */
			[[nodiscard]] const node* finish() const { return &_node; }

			/**
			 * When a node equals to the returned value, list traversal has ended.
			 * @return a node that's not in the list, is the node of the list itself.
			 */
			node* finish() { return &_node; }

			/**
			 * @return true iff the list is empty.
			 */
			[[nodiscard]] bool empty() const { return _node.empty(); }

			/**
			 * @return true iff the list is empty.
			 */
			bool empty() { return _node.empty(); }

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
			 * Removes (and returns) the last node of the list.
			 * @return the node removed from the tail of the list, nullptr if the list is empty.
			 */
			node* pop_back();

			/**
			 * Removes (and returns) the first node of the list.
			 * @return the node removed from the front of the list, nullptr if the list is empty.
			 */
			node* pop_front();

		};

	}
}

#endif //O1_LIB_DLL_HH
