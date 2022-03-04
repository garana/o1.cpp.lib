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


#ifndef O1CPPLIB_O1_BACKWARD_ITERATOR_HH
#define O1CPPLIB_O1_BACKWARD_ITERATOR_HH

#include <algorithm>
#include "./o1.iterator.hh"

namespace o1 {

	template <typename Node>
	class backward_iterator: public o1::iterator<Node> {
	public:
		explicit backward_iterator(Node* node): o1::iterator<Node>(node) { }

		backward_iterator(const backward_iterator<Node>& that) = default;

		backward_iterator(backward_iterator<Node>&& that) noexcept :
			o1::iterator<Node>(std::move(that)) {
		}

		bool operator == (const backward_iterator<Node>& that) const {
			return
				*static_cast<o1::iterator<Node>*>(this) ==
				static_cast<o1::iterator<Node>&>(that);
		}

		bool operator != (const backward_iterator<Node>& that) const {
			return
				*static_cast<o1::iterator<Node>*>(this) !=
				static_cast<o1::iterator<Node>&>(that);
		}

		// prefix
		backward_iterator<Node>& operator++() {
			if (this->_node != nullptr)
				this->_node = static_cast<Node*>(this->_node->prev());
			return backward_iterator(this->_node);
		}

	};

}

#endif //O1CPPLIB_O1_BACKWARD_ITERATOR_HH
