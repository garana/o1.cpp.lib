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

#ifndef O1CPPLIB_O1_LIST_D_LINKED_T_HH
#define O1CPPLIB_O1_LIST_D_LINKED_T_HH

#include "o1.list.d_linked.hh"
#include "../o1.member.hh"

namespace o1 {

	namespace list {

		template <typename Node, std::ptrdiff_t node_offset>
		class d_linked_t: protected o1::list::d_linked {

		public:
			class node: protected o1::list::d_linked::node {
			public:
				friend class d_linked_t<Node, node_offset>;
				node* next() { return static_cast<node*>(d_linked::node::next()); }
			};

		protected:
			using member = o1::member<Node, d_linked_t::node, node_offset>;

		public:

			class forward_iterator {
			public:
				friend class d_linked_t<Node, node_offset>;

			private:
				d_linked_t::node* _node = nullptr;

			protected:
				explicit forward_iterator(d_linked_t::node* node): _node(node) { }

			public:
				forward_iterator(const forward_iterator& that): _node(that._node) { }

				forward_iterator(forward_iterator&& that) noexcept : _node(std::move(that._node)) { }

				Node& operator*() { return *member::member2node(_node); }

				bool operator != (const forward_iterator& that) const {
					return _node != that._node;
				}

				// prefix
				forward_iterator operator++() {
					if (_node != nullptr)
						_node = _node->next();
					return forward_iterator(_node);
				}

			};

		public:

			d_linked_t() = default;

			d_linked_t(const d_linked_t& that) = delete;

			d_linked_t(d_linked_t&& that) noexcept:
				d_linked(std::move(that)) {
			}

			~d_linked_t() = default;

			forward_iterator begin() {
				return forward_iterator(static_cast<node*>(d_linked::start()));
			}

			forward_iterator end() {
				return forward_iterator(static_cast<node*>(d_linked::finish()));
			}

			using d_linked::empty;

			/**
			 * "Alias" of d_linked::push_back(node).
			 * Adds the element to the end of the list.
			 * @param node gets added at the end of the list.
			 */
			void push(Node* node) {
				d_linked::push_back(member::node2member(node));
			}

			/**
			 * "Alias" of d_linked::push_front(node).
			 * Adds the element to the head of the list.
			 * @param node gets added at the beginning of the list.
			 */
			void insert(Node* node) {
				d_linked::push_front(member::node2member(node));
			}

			/**
			 * "Alias" of d_linked::pop_back().
			 * Removes (and returns) the last element of the list.
			 * @return the removed element from the end of the list; or nullptr if it's empty.
			 */
			Node* pop() {
				return member::member2node(static_cast<d_linked_t::node*>(d_linked::pop_back()));
			}

			/**
			 * "Alias" of d_linked::pop_front().
			 * Removes (and returns) the first element of the list.
			 * @return the removed element from the head of the list, or nullptr if it's empty.
			 */
			Node* shift() {
				return member::member2node(static_cast<d_linked_t::node*>(d_linked::pop_front()));
			}

		};

	}

}

#endif //O1CPPLIB_O1_LIST_D_LINKED_T_HH
