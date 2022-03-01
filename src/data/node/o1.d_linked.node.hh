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


#ifndef O1CPPLIB_O1_D_LINKED_NODE_HH
#define O1CPPLIB_O1_D_LINKED_NODE_HH

#include <memory>

namespace o1 {

	namespace d_linked {

		class node;

		// TODO documentation
		class node {

		public:
			class EventHandlers {
			public:
				virtual void onAttach(node*) = 0;
				virtual void onDetach(node*) = 0;
			};

		private:
			node* _next = nullptr;
			node* _prev = nullptr;
			std::shared_ptr<EventHandlers> _handlers = nullptr;
			void _push_back(node* node);
			void _onAttach(node* node);

		public:
			node();

			explicit node(std::shared_ptr<EventHandlers> handlers);

			node(const node& that) = delete;

			/**
			 * Move constructor.
			 * Replace @param that with *this on the list.
			 * @param that
			 */
			node(node&& that) noexcept;

			inline node* eventHandlers(std::shared_ptr<EventHandlers> handlers) {
				_handlers = handlers;
				return this;
			}

			/**
			 * Detach this node on destructor.
			 */
			~node() { detach(); }

			void detach();

			[[nodiscard]] inline const node* next() const { return _next; }

			inline node* next() { return _next; }

			[[nodiscard]] inline const node* prev() const { return _prev; }

			inline node* prev() { return _prev; }

			/**
			 * Inserts @param next after *this.
			 */
			void push_back(node* next);

			/**
			 * Inserts @param node before *this.
			 */
			void push_front(node* node);

			/**
			 * @return if this node is the only one in the list.
			 */
			bool empty();

			/**
			 * @return if this node is the only one in the list.
			 */
			bool empty() const;

		};

	}
}

#endif //O1CPPLIB_O1_D_LINKED_NODE_HH
