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


#include <utility>
#include "o1.d_linked.node.hh"
#include "../../o1.debug.hh"
#include "../../o1.logging.hh"

using node = o1::d_linked::node;

node::node() {
	_next = _prev = this;
	_handlers = nullptr;
}

node::node(std::shared_ptr<node::EventHandlers> handlers):
	_handlers(std::move(handlers)) {
	_next = _prev = this;
}

node::node(class node&& that) noexcept:
	_next(that._next),
	_prev(that._prev),
	_handlers(std::move(that._handlers)) {

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
	if (!empty()) {
		_next->_prev = _prev;
		_prev->_next = _next;
		_prev = _next = this;

		if (_handlers != nullptr)
			_handlers->onDetach(this);
		_handlers = nullptr;
	}
}


void
node::_push_back(node* node) {
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
node::_onAttach(node* node) {
	node->_handlers = _handlers;
	if (_handlers != nullptr)
		_handlers->onAttach(node);
}

void
node::push_back(node* node) {
	_push_back(node);
	_onAttach(node);
}


void
node::push_front(node* node) {
	node->_push_back(this);
	_onAttach(node);
}

bool node::empty() {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			(_next == this) == (_next == _prev),
			"o1::d_linked::node::empty: inconsistency in node"
		);
	}
	return _next == this;
}

bool node::empty() const {
	if (o1::flags::extended_checks()) {
		o1::xassert(
			(_next == this) == (_next == _prev),
			"o1::d_linked::node::empty: inconsistency in node"
		);
	}
	return _next == this;
}
