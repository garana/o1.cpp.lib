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
#include "o1.d_linked.list.hh"

namespace {

	TEST(o1_d_linked, Constructor) {
		o1::d_linked::list list;
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);
	}


	TEST(o1_d_linked, MoveConstructorEmptyList) {
		o1::d_linked::list src;
		o1::d_linked::list dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
		EXPECT_EQ(src.size(), 0);
		EXPECT_EQ(dst.size(), 0);
	}


	TEST(o1_d_linked, MoveConstructorNonEmptyList) {
		o1::d_linked::list src;
		o1::d_linked::node node;
		src.push_back(&node);
		EXPECT_EQ(src.size(), 1);
		o1::d_linked::list dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
		EXPECT_EQ(src.size(), 0);
		EXPECT_EQ(dst.size(), 1);
	}


	TEST(o1_d_linked, PushBack) {
		o1::d_linked::list list;
		o1::d_linked::node node;
		EXPECT_EQ(list.size(), 0);
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
		EXPECT_EQ(list.size(), 1);
	}


	TEST(o1_d_linked, PushFront) {
		o1::d_linked::list list;
		o1::d_linked::node node;
		EXPECT_EQ(list.size(), 0);
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
		EXPECT_EQ(list.size(), 1);
	}

	TEST(o1_d_linked, PopFrontAfterPushBack) {
		o1::d_linked::list list;
		o1::d_linked::node node;

		EXPECT_EQ(list.size(), 0);

		list.push_back(&node);
		EXPECT_EQ(list.size(), 1);
		EXPECT_FALSE(list.empty());

		auto front = list.pop_front();
		EXPECT_EQ(list.size(), 0);
		EXPECT_TRUE(list.empty());

		auto tail = list.pop_back();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);
	}


	TEST(o1_d_linked, PopBackAfterPushBack) {
		o1::d_linked::list list;
		o1::d_linked::node node;

		EXPECT_EQ(list.size(), 0);
		list.push_back(&node);
		EXPECT_FALSE(list.empty());
		EXPECT_EQ(list.size(), 1);

		auto tail = list.pop_back();
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);

		auto front = list.pop_back();
		EXPECT_EQ(front, nullptr);
		EXPECT_EQ(tail, &node);
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);
	}


	TEST(o1_d_linked, PopFrontAfterPushFront) {
		o1::d_linked::list list;
		o1::d_linked::node node;

		EXPECT_EQ(list.size(), 0);
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
		EXPECT_EQ(list.size(), 1);

		auto front = list.pop_front();
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);

		auto tail = list.pop_back();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
		EXPECT_EQ(list.size(), 0);
	}


	TEST(o1_d_linked, PopBackAfterPushFront) {
		o1::d_linked::list list;
		o1::d_linked::node node;

		EXPECT_EQ(list.size(), 0);
		list.push_front(&node);
		EXPECT_FALSE(list.empty());
		EXPECT_EQ(list.size(), 1);

		auto tail = list.pop_back();
		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);

		auto front = list.pop_front();
		EXPECT_EQ(front, nullptr);
		EXPECT_EQ(tail, &node);
		EXPECT_EQ(list.size(), 0);
	}


	TEST(o1_d_linked, PushBackAfterMove) {
		o1::d_linked::list src;
		o1::d_linked::node nodes[3];

		EXPECT_EQ(src.size(), 0);

		src.push_back(&nodes[0]);

		EXPECT_FALSE(src.empty());
		EXPECT_EQ(src.size(), 1);

		o1::d_linked::list dst(std::move(src));
		EXPECT_EQ(src.size(), 0);
		EXPECT_EQ(dst.size(), 1);

		src.push_back(&nodes[1]);
		EXPECT_EQ(src.size(), 1);
		EXPECT_EQ(dst.size(), 1);

		dst.push_back(&nodes[2]);
		EXPECT_EQ(src.size(), 1);
		EXPECT_EQ(dst.size(), 2);
	}


	TEST(o1_d_linked, PushFrontAfterMove) {
		o1::d_linked::list src;
		o1::d_linked::node nodes[3];

		EXPECT_EQ(src.size(), 0);

		src.push_front(&nodes[0]);

		EXPECT_FALSE(src.empty());
		EXPECT_EQ(src.size(), 1);

		o1::d_linked::list dst(std::move(src));
		EXPECT_EQ(src.size(), 0);
		EXPECT_EQ(dst.size(), 1);

		src.push_front(&nodes[1]);
		EXPECT_EQ(src.size(), 1);
		EXPECT_EQ(dst.size(), 1);

		dst.push_front(&nodes[2]);
		EXPECT_EQ(src.size(), 1);
		EXPECT_EQ(dst.size(), 2);
	}


	TEST(o1_d_linked, ForwardLoop) {
		o1::d_linked::list list;
		o1::d_linked::node nodes[10];

		int inode = 0;
		for (auto& i: nodes) {
			EXPECT_EQ(list.size(), inode);
			list.push_back(&i);
			++inode;
			EXPECT_EQ(list.size(), inode);
		}

		inode = 0;
		for (o1::d_linked::node* node = list.start();
			 node != list.finish();
			 ++inode, node = node->next()) {
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_LT(inode, 10) << "inode=" << inode << " vs " << (node - &node[0]);
		}

	}

	TEST(o1_d_linked, ReverseLoop) {
		o1::d_linked::list list;
		o1::d_linked::node nodes[10];

		int inode = 0;

		for (auto& i: nodes) {
			EXPECT_EQ(list.size(), inode);
			list.push_front(&i);
			++inode;
			EXPECT_EQ(list.size(), inode);
		}

		inode = 9;
		for (o1::d_linked::node* node = list.r_start();
			 node != list.finish();
			 --inode, node = node->prev()) {
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_GE(inode, 0) << "inode=" << inode << " vs " << (node - &node[0]);
		}

	}


	TEST(o1_d_linked, SizeConsistency) {
		o1::d_linked::list list;
		EXPECT_EQ(list.size(), 0);

		{
			o1::d_linked::node node;
			list.push_back(&node);
			EXPECT_FALSE(list.empty());
			EXPECT_EQ(list.size(), 1);
		}

		EXPECT_TRUE(list.empty());
		EXPECT_EQ(list.size(), 0);
	}

}
