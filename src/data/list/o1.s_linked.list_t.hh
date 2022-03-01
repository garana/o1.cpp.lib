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


#ifndef O1CPPLIB_O1_S_LINKED_LIST_T_HH
#define O1CPPLIB_O1_S_LINKED_LIST_T_HH

#include "./o1.s_linked.list.hh"
#include "../node/o1.s_linked.node_t.hh"
#include "../iterator/o1.forward_iterator_ref.hh"

namespace o1 {

	namespace s_linked {

		template<typename T>
		class list_t : protected o1::s_linked::list {

		public:
			using node = list::node;
			using getNodeFn = typename o1::s_linked::node_t<T>::getNodeFn;

		private:
			getNodeFn getNode{nullptr};

		public:

			list_t() = delete;

			explicit list_t(getNodeFn _getNode) : getNode(_getNode) {}

			list_t(const list_t& that) = delete;

			list_t(list_t&& that) noexcept:
				o1::s_linked::list(std::move(that)),
				getNode(that.getNode) {
				that.getNode = nullptr;
			}

			~list_t() = default;

			o1::forward_iterator_ref<list::node, T> begin() {
				return o1::forward_iterator_ref<list::node, T>(
					static_cast<node_t<T>*>(
						list::start()
					)
				);
			}

			o1::forward_iterator_ref<list::node, T> end() {
				return o1::forward_iterator_ref<list::node, T>(
					static_cast<node_t<T>*>(
						nullptr
					)
				);
			}

			using s_linked::list::empty;

			/**
			 * "Alias" of d_linked::push_back(node).
			 * Adds the element to the end of the list.
			 * @param node gets added at the end of the list.
			 */
			void push_back(T* datum) {
				s_linked::list::push_back(getNode(datum));
			}

			/**
			 * "Alias" of d_linked::push_front(node).
			 * Adds the element to the head of the list.
			 * @param node gets added at the beginning of the list.
			 */
			void push_front(T* datum) {
				s_linked::list::push_front(getNode(datum));
			}

			/**
			 * "Alias" of d_linked::pop_front().
			 * Removes (and returns) the first element of the list.
			 * @return the removed element from the head of the list, or nullptr if it's empty.
			 */
			T* pop_front() {
				return node_t<T>::ref(
					static_cast<node_t<T>*>(s_linked::list::pop_front())
				);
			}

		};

	}

}

#endif //O1CPPLIB_O1_S_LINKED_LIST_T_HH
