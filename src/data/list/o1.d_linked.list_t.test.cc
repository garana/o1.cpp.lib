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
#include "o1.d_linked.list_t.hh"

namespace {

	struct MyNode {
		int value{};
		o1::d_linked::list_t<MyNode>::node_t node;

		MyNode() : node(this) {};
	};

	o1::d_linked::list_t<MyNode>::node_t* getNode(MyNode* datum) {
		return &datum->node;
	}

	using list_t = o1::d_linked::list_t<MyNode>;

	TEST(o1_d_linked_t, Constructor) {
		list_t list(getNode);
		EXPECT_TRUE(list.empty());
	}


	TEST(o1_d_linked_t, MoveConstructorEmptyList) {
		list_t src(getNode);
		list_t dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
	}


	TEST(o1_d_linked_t, MoveConstructorNonEmptyList) {
		list_t src(getNode);
		MyNode node;
		src.push_back(&node);
		list_t dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
	}


	TEST(o1_d_linked_t, PushBack) {
		list_t list(getNode);
		MyNode node;
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
	}


	TEST(o1_d_linked_t, PushFront) {
		list_t list(getNode);
		MyNode node;
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
	}

	TEST(o1_d_linked_t, PopFrontAfterPushBack) {
		list_t list(getNode);
		MyNode node;
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
		auto front = list.pop_front();
		EXPECT_TRUE(list.empty());
		auto tail = list.pop_back();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
		EXPECT_TRUE(list.empty());
	}


	TEST(o1_d_linked_t, PopBackAfterPushBack) {
		list_t list(getNode);
		MyNode node;
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
		auto tail = list.pop_back();
		EXPECT_TRUE(list.empty());
		auto front = list.pop_back();
		EXPECT_EQ(front, nullptr);
		EXPECT_EQ(tail, &node);
		EXPECT_TRUE(list.empty());
	}


	TEST(o1_d_linked_t, PopFrontAfterPushFront) {
		list_t list(getNode);
		MyNode node;
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
		auto front = list.pop_front();
		EXPECT_TRUE(list.empty());
		auto tail = list.pop_back();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
	}


	TEST(o1_d_linked_t, PopBackAfterPushFront) {
		list_t list(getNode);
		MyNode node;
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
		auto tail = list.pop_back();
		EXPECT_TRUE(list.empty());
		auto front = list.pop_front();
		EXPECT_EQ(front, nullptr);
		EXPECT_EQ(tail, &node);
	}


	TEST(o1_d_linked_t, ForwardLoop) {
		list_t list(getNode);
		MyNode nodes[10];
		int inode;

		inode = 0;
		for (auto& i: nodes) {
			i.value = inode;
			list.push_back(&i);
			++inode;
		}

		inode = 0;
		for (auto i: list) {
			EXPECT_EQ(i->value, nodes[inode].value);
			EXPECT_LT(inode, 10) << "inode=" << inode;
			++inode;
		}

	}

}
