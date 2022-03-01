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


#include <gtest/gtest.h>
#include "o1.data.hh"
#include "o1.int.hh"

#define SHOW(s)  std::cout << (#s) << ": " << s << "\n";

namespace {

	class C {

	};

	class D: public C {

	};

	class V {
	public:
		virtual ~V() = default;
	};

	class W: public V {
	public:
		~W() override = default;
	};

	TEST(print, size_of) {
		SHOW(sizeof(int));
		SHOW(sizeof(long));
		SHOW(sizeof(long long));
		SHOW(sizeof(int128_t));
		SHOW(sizeof(void*));
		SHOW(sizeof(std::function<void()>));
		SHOW(sizeof(std::function<void()>*));
		SHOW(sizeof(std::function<void(int)>));
		SHOW(sizeof(o1::s_linked::list));
		SHOW(sizeof(o1::s_linked::list::node));
		SHOW(sizeof(o1::d_linked::list));
		SHOW(sizeof(o1::d_linked::list::node));
		SHOW(sizeof(o1::s_linked::queue));
		SHOW(sizeof(o1::s_linked::queue::node));
		SHOW(sizeof(o1::d_linked::queue));
		SHOW(sizeof(o1::d_linked::queue::node));
		SHOW(sizeof(o1::s_linked::stack));
		SHOW(sizeof(o1::s_linked::stack::node));
		SHOW(sizeof(o1::d_linked::stack));
		SHOW(sizeof(o1::d_linked::stack::node));
		SHOW(sizeof(o1::d_linked::stack::node));
		SHOW(sizeof(C));
		SHOW(sizeof(D));
		SHOW(sizeof(V));
		SHOW(sizeof(W));
	}

}
