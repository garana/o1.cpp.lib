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

#include "o1.s_linked.list.hh"
#include "../../o1.debug.hh"
#include "../../o1.logging.hh"

using o1::s_linked::list;
using node = o1::s_linked::list::node;

list::list(list&& that) noexcept {
	_head.move(that._head);

	that._tail = &that._head;
	_tail = &_head;
}

void
list::push_back(node* node) {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			node->next() == nullptr,
			"o1::list::s_linked::push_back: node to push must be empty"
		);
	}

	_tail->next(node);
	_tail = node;
}

void
list::push_front(node* node) {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			node->next() == nullptr,
			"o1::list::s_linked::push_front: node to push must be empty"
		);
	}

	s_linked::node* tmp = _head.next();
	_head.next(node);
	node->next(tmp);
}


node*
list::pop_front() {
	node* retVal = _head.next();

	if (retVal != nullptr) {
		_head.next(_head.next()->next());
		retVal->next(nullptr);
		return retVal;
	}

	_tail = &_head;
	return nullptr;
}
