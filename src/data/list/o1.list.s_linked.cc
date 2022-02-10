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

#include "o1.list.s_linked.hh"
#include "../../o1.debug.hh"
#include "../../o1.logging.hh"

using o1::list::s_linked;
using node = o1::list::s_linked::node;

o1::list::s_linked::s_linked(s_linked&& that) noexcept {
	_head._next = that._head._next;
	that._head._next = nullptr;

	that._tail = &that._head;
	_tail = &_head;
}

void
s_linked::push_back(node* node) {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			node->_next == nullptr,
			"o1::list::s_linked::push_back: node to push must be empty"
		);
	}

	_tail->_next = node;
	_tail = node;
}

void
s_linked::push_front(node* node) {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			node->_next == nullptr,
			"o1::list::s_linked::push_front: node to push must be empty"
		);
	}

	s_linked::node* tmp = _head._next;
	_head._next = node;
	node->_next = tmp;
}


node*
s_linked::pop_front() {
	node* retVal = _head.next();

	if (retVal != nullptr) {
		_head._next = _head._next->_next;
		return retVal;
	}

	_tail = &_head;
	return nullptr;
}
