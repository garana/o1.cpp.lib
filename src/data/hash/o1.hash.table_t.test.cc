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
#include "o1.hash.table_t.hh"

namespace {

	struct HashNode {
		int key;

		mutable o1::hash::list_t<HashNode>::node hash_node;

		explicit HashNode(int _key) : key(_key), hash_node(this) {}
	};

	using Key = decltype(HashNode::key);
	using Value = HashNode;
	using node = typename o1::hash::list_t<Value>::node;

	o1::hash::hash_val hashFn(const Key& key) {
		return o1::hash::hashValue(&key, sizeof(key), 0);
	}

	const Key getKey(const Value* value) {
		return value->key;
	}

	node* getNode(Value* value) {
		return &value->hash_node;
	}

	bool equalFn(const Key& left, const Key& right) {
		return left == right;
	}

	o1::hash::ops<int, HashNode> _hash_ops{
		.hashValue = hashFn,
		.getKey = getKey,
		.getNode = getNode,
		.equal = equalFn
	};


	TEST(o1_hash_table, basic_tests) {

		o1::hash::table<Key, Value, &_hash_ops> table;

		HashNode node{1};
		bool inserted = table.insert(&node);
		HashNode* found = table.find(getKey(&node));
		HashNode* found1 = table.find(1);
		bool removed = table.remove(&node);
		HashNode* notFound = table.find(4);

		EXPECT_EQ(inserted, true);
		EXPECT_EQ(found, &node);
		EXPECT_EQ(found1, &node);
		EXPECT_EQ(removed, true);
		EXPECT_EQ(notFound, nullptr);
	}

	TEST(o1_hash_table, rehashing) {
#define NODE_COUNT 513

		o1::hash::table<Key, Value, &_hash_ops> table;

		HashNode* nodes[NODE_COUNT]{nullptr};

		for (int i = 0; i < NODE_COUNT; ++i) {
			nodes[i] = new HashNode(i);
			EXPECT_EQ(table.insert(nodes[i]), true);
			for (int j = 0; j < NODE_COUNT; ++j) {
				EXPECT_EQ(table.find(j), nodes[j])
					<< "(i,j)=" << i << ',' << j << "\n";
			}
		}

		for (int i = 0; i < NODE_COUNT; ++i) {
			EXPECT_EQ(table.remove(nodes[i]), true);
			delete nodes[i];
			nodes[i] = nullptr;

			for (int j = 0; j < NODE_COUNT; ++j) {
				EXPECT_EQ(table.find(j), nodes[j])
					<< "(i,j)=" << i << ',' << j << "\n";
			}
		}

	}

}