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

#include "o1.d_linked.list.hh"

using o1::d_linked::list;
using node = o1::d_linked::list::node;


void o1::d_linked::list::_quick_move_to(list* list) {
	_nodeEventHandlers->setList(list);
	_nodeEventHandlers = std::make_shared<NodeEventHandlers>(this);
	_node.reset(_nodeEventHandlers);
}

list::list(list::EventHandlers* handlers):
	_listEventHandlers(handlers) {
}

list::list(list&& that) noexcept:
	_numElements(that._numElements),
	_listEventHandlers(that._listEventHandlers),
	_node(std::move(that._node)) {
	that._numElements = 0;
	that._quick_move_to(this);
}

void list::flush() {
	_quick_move_to(nullptr);
}

node*
list::pop_front() {
	node* next = _node.next();

	if (next != &_node) {
		next->detach();
		return next;
	}

	return nullptr;
}


node* list::pop_back() {
	node* prev = _node.prev();

	if (prev != &_node) {
		prev->detach();
		return prev;
	}

	return nullptr;
}

