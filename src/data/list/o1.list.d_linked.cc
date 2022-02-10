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

#include "o1.list.d_linked.hh"
#include "../../o1.debug.hh"
#include "../../o1.logging.hh"

using o1::list::d_linked;
using node = d_linked::node;

node::node(class node&& that) noexcept:
	_next(that._next),
	_prev(that._prev) {

	if (that._prev == &that) {
		_prev = this;
	} else {
		that._prev->_next = this;
	}

	if (that._next == &that) {
		_next = this;
	} else {
		that._next->_prev = this;
	}

	that._next = that._prev = &that;
}


void
node::detach() {
	_next->_prev = _prev;
	_prev->_next = _next;
	_prev = _next = this;
}


void
node::push_back(node* node) {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			node->empty(),
			"o1.list.d_linked::node::push_back called with non-empty node"
		);
	}

	node->_prev = _prev;
	_prev->_next = node;
	_prev = node;
	node->_next = this;
}


void
node::push_front(node* next) {
	next->push_back(this);
}

bool o1::list::d_linked::node::empty() {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			(_next == this) == (_next == _prev),
			"o1::list::d_linked::node::empty: inconsistency in node"
		);
	}
	return _next == this;
}

bool o1::list::d_linked::node::empty() const {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			(_next == this) == (_next == _prev),
			"o1::list::d_linked::node::empty: inconsistency in node"
		);
	}
	return _next == this;
}


void
d_linked::push_back(node* node) {
	_node.push_back(node);
}

void d_linked::push_front(node* node) {
	_node.push_front(node);
}


node*
d_linked::pop_front() {
	node* next = _node.next();

	if (next != &_node) {
		next->detach();
		return next;
	}

	return nullptr;
}


node* d_linked::pop_back() {
	node* prev = _node.prev();

	if (prev != &_node) {
		prev->detach();
		return prev;
	}

	return nullptr;
}
