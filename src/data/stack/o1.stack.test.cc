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
#include "o1.stack.hh"

namespace {

	TEST(o1_stack, Constructor) {
		o1::stack stack;
		EXPECT_TRUE(stack.empty());
	}


	TEST(o1_stack, MoveConstructorEmptyList) {
		o1::stack src;
		o1::stack dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
	}

	TEST(o1_stack, MoveConstructorNonEmptyList) {
		o1::stack src;
		o1::stack::node node;
		src.push(&node);
		o1::stack dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
	}

	TEST(o1_stack, Push) {
		o1::stack stack;
		o1::stack::node node;
		stack.push(&node);
		EXPECT_FALSE(stack.empty());
	}


	TEST(o1_stack, PopAfterPush) {
		o1::stack stack;
		o1::stack::node node;
		stack.push(&node);
		EXPECT_FALSE(stack.empty());
		auto front = stack.pop();
		EXPECT_TRUE(stack.empty());
		auto tail = stack.pop();
		EXPECT_EQ(front, &node);
		EXPECT_EQ(tail, nullptr);
		EXPECT_TRUE(stack.empty());
	}

	TEST(o1_stack, Loop) {
		o1::stack stack;
		o1::stack::node nodes[10];

		for (auto& i: nodes) {
			stack.push(&i);
		}

		int inode = 9;
		while (o1::stack::node* node = stack.peek()) {
			const o1::stack::node* const_node = const_cast<const o1::stack&>(stack).peek();
			o1::stack::node* top = stack.pop();
			EXPECT_EQ(node, const_node) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, top) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_GE(inode, 0) << "inode=" << inode << " vs " << (node - &node[0]);
			--inode;
		}

	}

}
