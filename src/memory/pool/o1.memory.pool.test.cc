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
#include "o1.memory.pooled.hh"

namespace {

	class Obj: public o1::memory::pooled<Obj, o1::memory::PoolStrategy::chunkedAlloc> {
		int x[15];
	public:
		Obj() = default;
		Obj(int _value) {
			x[0] = _value;
			x[14] = _value;
		}
	};


	TEST(o1_memory_pooled, basic) {
		delete (new Obj());
		Obj* v[10];

		for (int i = 10; i--;) {
			v[i] = new Obj();
		}

		for (int i = 10; i--;) {
			delete v[i];
		}
	}


	TEST(o1_memory_pooled, torture) {
		delete (new Obj());
		Obj* v[10000];
		memset(&v[0], 0, sizeof(v));

		for (int loop = 1000000; loop--;) {
			int i = rand() % 10000;
			if (v[i] != nullptr) {
				delete v[i];
				v[i] = nullptr;
			} else {
				v[i] = new Obj(i);
			}
		}

	}

}