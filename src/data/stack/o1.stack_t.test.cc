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
#include "o1.stack_t.hh"

namespace {

	struct MyNode {
		int value{};
		o1::stack::node node;
	};

	using stack_t = o1::stack_t<MyNode, offsetof(MyNode, node)>;


	TEST(o1_stack, Constructor) {
		stack_t stack;
		EXPECT_TRUE(stack.empty());
	}


	TEST(o1_stack, MoveConstructorEmptyList) {
		stack_t src;
		stack_t dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
	}

	TEST(o1_stack, MoveConstructorNonEmptyList) {
		stack_t src;
		MyNode node;
		src.push(&node);
		stack_t dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
	}

	TEST(o1_stack, Push) {
		stack_t stack;
		MyNode node;
		stack.push(&node);
		EXPECT_FALSE(stack.empty());
	}


	TEST(o1_stack, PopAfterPush) {
		stack_t stack;
		MyNode node;
		stack.push(&node);
		EXPECT_FALSE(stack.empty());
		auto head = stack.pop();
		EXPECT_TRUE(stack.empty());
		auto none = stack.pop();
		EXPECT_EQ(head, &node);
		EXPECT_EQ(none, nullptr);
		EXPECT_TRUE(stack.empty());
	}

	TEST(o1_stack, Loop) {
		stack_t stack;
		MyNode nodes[10];
		int inode;

		inode = 0;
		for (auto& i: nodes) {
			i.value = inode;
			stack.push(&i);
		}

		inode = 9;
		while (MyNode* node = stack.peek()) {
			const MyNode* const_node = const_cast<const stack_t&>(stack).peek();
			MyNode* head = stack.pop();
			EXPECT_EQ(node, const_node) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, head) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_LT(inode, 10) << "inode=" << inode << " vs " << (node - &node[0]);
			--inode;
		}

	}

}
