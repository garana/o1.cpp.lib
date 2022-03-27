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

#ifndef O1CPPLIB_O1_D_LINKED_LIST_HH
#define O1CPPLIB_O1_D_LINKED_LIST_HH

#include <cstddef>
#include <algorithm>
#include <utility>
#include <memory>
#include "../node/o1.d_linked.node.hh"

namespace o1 {

	namespace d_linked {

		/**
		 * Double (circular) Linked List.
		 *
		 * If a node gets destroyed (it's destructor called) before the list itself does,
		 * the list remains consistent, as each node detaches it self from the list.
		 * This means that if the list node is a member of the contained element,
		 * when the latter gets destroyed, it gets detached from the list,
		 * hence it keeps consistency.
		 *
		 * These operations are supported (all O(1)):
		 * - push_back (push, append)
		 * - push_front (insert)
		 * - pop_back (pop)
		 * - pop_front (shift)
		 * - Any random node detached.
		 *
		 */
		class list {

		public:
			using node = o1::d_linked::node;
			using EventHandlers = o1::ContainerEventHandlers<node, list>;

		private:

			class NodeEventHandlers: public node::EventHandlers {
				list* _list{nullptr};
				friend list;
			public:
				explicit NodeEventHandlers(list* list): _list(list) { }

				void setList(list* newList) {
					_list = newList;
				}

				void attaching(node* node) override {
					if (_list && _list->_listEventHandlers)
						_list->_listEventHandlers->attaching(node, _list);
				}

				void attached(node* node) override {
					_list->_numElements++;
					if (_list && _list->_listEventHandlers)
						_list->_listEventHandlers->attached(node, _list);
				}

				void detaching(node* node) override {
					if (_list && _list->_listEventHandlers)
						_list->_listEventHandlers->detaching(node, _list);
				}

				void detached(node* node) override {
					_list->_numElements--;
					if (_list && _list->_listEventHandlers)
						_list->_listEventHandlers->detached(node, _list);
				}
			};

			size_t _numElements{0};
			EventHandlers* _listEventHandlers{nullptr};
			std::shared_ptr<NodeEventHandlers> _nodeEventHandlers{
				std::make_shared<NodeEventHandlers>(this)
			};
			node _node{_nodeEventHandlers};

			void _quick_move_to(list* list);

		public:
			list() = default;

			explicit list(EventHandlers* handlers);

			list(const list& that) = delete;

			list(list&& that) noexcept;

			/**
			 * Remove all nodes from the list, NOT deleting them.
			 */
			void flush();

			/**
			 * Upon destruction, entries are NOT deleted.
			 * They'll form a  double linked list on their own.
			 */
			virtual ~list() = default;

			/**
			 * First node of the list.
			 * @return the first node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] inline const node* start() const { return _node.next(); }

			/**
			 * First node of the list.
			 * @return the first node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] inline node* start() { return _node.next(); }

			/**
			 * Last element of the list.
			 * @return the last node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] inline const node* r_start() const { return _node.prev(); }

			/**
			 * Last element of the list.
			 * @return the last node of the list, or finish() if it's empty.
			 */
			[[nodiscard]] inline node* r_start() { return _node.prev(); }

			/**
			 * When a node equals to the returned value, list traversal has ended.
			 * @return a node that's not in the list, is the node of the list itself.
			 */
			[[nodiscard]] inline const node* finish() const { return &_node; }

			/**
			 * When a node equals to the returned value, list traversal has ended.
			 * @return a node that's not in the list, is the node of the list itself.
			 */
			[[nodiscard]] inline node* finish() { return &_node; }

			/**
			 * @return true iff the list is empty.
			 */
			[[nodiscard]] inline bool empty() const { return _node.empty(); }

			/**
			 * @return true iff the list is empty.
			 */
			[[nodiscard]] inline bool empty() { return _node.empty(); }

			/**
			 * @return true iff the list is empty.
			 */
			[[nodiscard]] inline size_t size() const { return _numElements; }

			/**
			 * Adds the @param node to the end of the list.
			 * @param node node inserted at the end of the list.
			 */
			inline void push_back(node* node) {
				_node.push_back(node);
			}

			/**
			 * Inserts @param node at the beginning of the list.
			 * @param node node inserted at the beginning of the list.
			 */
			inline void push_front(node* node) {
				_node.push_front(node);
			}

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

#endif //O1CPPLIB_O1_D_LINKED_LIST_HH
