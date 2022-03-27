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
#include "o1.s_linked.list.hh"

namespace {

	TEST(o1_s_linked, Constructor) {
		o1::s_linked::list list;
		EXPECT_TRUE(list.empty());
	}


	TEST(o1_s_linked, MoveConstructorEmptyList) {
		o1::s_linked::list src;
		o1::s_linked::list dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
	}

	TEST(o1_s_linked, MoveConstructorNonEmptyList) {
		o1::s_linked::list src;
		o1::s_linked::node node;
		src.push_back(&node);
		o1::s_linked::list dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
	}

	TEST(o1_s_linked, PushBack) {
		o1::s_linked::list list;
		o1::s_linked::node node;
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
	}


	TEST(o1_s_linked, PushFront) {
		o1::s_linked::list list;
		o1::s_linked::node node;
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
	}

	TEST(o1_s_linked, PopFrontAfterPushBack) {
		o1::s_linked::list list;
		o1::s_linked::node node;
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
		auto front = list.pop_front();
		EXPECT_TRUE(list.empty());
		auto tail = list.pop_front();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
		EXPECT_TRUE(list.empty());
	}

	TEST(o1_s_linked, ForwardLoop) {
		o1::s_linked::list list;
		o1::s_linked::node nodes[10];

		for (auto& i: nodes) {
			list.push_back(&i);
		}

		int inode = 0;
		for (o1::s_linked::node* node = list.start();
			 node != nullptr;
			 ++inode, node = node->next()) {
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_LT(inode, 10) << "inode=" << inode << " vs " << (node - &node[0]);
		}

	}

}
