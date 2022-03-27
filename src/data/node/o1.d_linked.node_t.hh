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


#ifndef O1CPPLIB_O1_D_LINKED_NODE_T_HH
#define O1CPPLIB_O1_D_LINKED_NODE_T_HH

#include "./o1.node_t.hh"
#include "./o1.d_linked.node.hh"

namespace o1 {

	namespace d_linked {

		// TODO documentation
		template <typename T>
		class node_t: public o1::node_t<T>, public o1::d_linked::node {
		public:
			node_t() = delete;
			explicit node_t(T* ref): o1::node_t<T>(ref) { }
			~node_t() override = default;

			/**
			 * Given the datum object, return the o1::d_linked::node_t<T>.
			 */
			using getNodeFn = node_t<T>* (*)(T* obj);

//			using o1::d_linked::node::detach;
//			using o1::d_linked::node::empty;
//			using o1::d_linked::node::push_back;
//			using o1::d_linked::node::push_front;
//
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "HidingNonVirtualFunction"
//			[[nodiscard]] inline const node_t* next() const {
//				return dynamic_cast<const node_t<T>*>(
//					d_linked::node::next()
//				);
//			}
//
//			[[nodiscard]] inline node_t* next() {
//				return dynamic_cast<node_t<T>*>(
//					d_linked::node::next()
//				);
//			}
//
//			[[nodiscard]] inline const node_t* prev() const {
//				return dynamic_cast<const node_t<T>*>(
//					d_linked::node::prev()
//				);
//			}
//
//			[[nodiscard]] inline node_t* prev() {
//				return dynamic_cast<node_t<T>*>(
//					d_linked::node::prev()
//				);
//			}
//#pragma clang diagnostic pop

		};

	}

}

#endif //O1CPPLIB_O1_D_LINKED_NODE_T_HH
