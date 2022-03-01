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
#include "o1.s_linked.queue_t.hh"

// TODO all UTs in namespace{}
namespace {

	// TODO define this for list, queue & stack
	template <typename T>
	using queue_t = o1::s_linked::queue_t<T>;

	template <typename T>
	using node = typename queue_t<T>::node;

	struct MyNode {

		int value{};
		node<MyNode> _node;

		MyNode(): _node(this) { }

		static ::node<MyNode>* getNode(MyNode* that) {
			return &that->_node;
		}
	};


	TEST(o1_queue_t, Constructor) {
		queue_t<MyNode> queue(MyNode::getNode);
		EXPECT_TRUE(queue.empty());
	}


	TEST(o1_queue_t, MoveConstructorEmptyList) {
		queue_t<MyNode> src(MyNode::getNode);
		queue_t<MyNode> dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_TRUE(dst.empty());
	}

	TEST(o1_queue_t, MoveConstructorNonEmptyList) {
		queue_t<MyNode> src(MyNode::getNode);
		MyNode node;
		src.push(&node);
		queue_t<MyNode> dst(std::move(src));
		EXPECT_TRUE(src.empty()); // NOLINT(bugprone-use-after-move)
		EXPECT_FALSE(dst.empty());
	}

	TEST(o1_queue_t, Push) {
		queue_t<MyNode> queue(MyNode::getNode);
		MyNode node;
		queue.push(&node);
		EXPECT_FALSE(queue.empty());
	}


	TEST(o1_queue_t, PopAfterPush) {
		queue_t<MyNode> queue(MyNode::getNode);
		MyNode node;
		queue.push(&node);
		EXPECT_FALSE(queue.empty());
		auto head = queue.pop();
		EXPECT_TRUE(queue.empty());
		auto none = queue.pop();
		EXPECT_EQ(head, &node);
		EXPECT_EQ(none, nullptr);
		EXPECT_TRUE(queue.empty());
	}

	TEST(o1_queue_t, Loop) {
		queue_t<MyNode> queue(MyNode::getNode);
		MyNode nodes[10];

		for (auto& i: nodes) {
			queue.push(&i);
		}

		int inode = 0;
		while (MyNode* node = queue.peek()) {
			const MyNode* const_node = const_cast<const queue_t<MyNode>&>(queue).peek();
			MyNode* head = queue.pop();
			EXPECT_EQ(node, const_node) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, head) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_EQ(node, &nodes[inode]) << "inode=" << inode << " vs " << (node - &node[0]);
			EXPECT_LT(inode, 10) << "inode=" << inode << " vs " << (node - &node[0]);
			++inode;
		}

	}

}
