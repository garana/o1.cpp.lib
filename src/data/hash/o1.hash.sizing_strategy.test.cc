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
#include "o1.hash.sizing_strategy.hh"

TEST(o1_hash_sizing_strategy, bucketsSizing) {

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(0, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 0);
		EXPECT_EQ(numBuckets, 8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 0);
		EXPECT_EQ(numBuckets, 8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8*8, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 0);
		EXPECT_EQ(numBuckets, 8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8*8+1, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 1);
		EXPECT_EQ(numBuckets, 8*8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8*8*8, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 1);
		EXPECT_EQ(numBuckets, 8*8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8*8*8+1, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 2);
		EXPECT_EQ(numBuckets, 8*8*8);
	}

	{
		size_t size_index;
		size_t numBuckets;
		o1::hash::sizing_strategy::bucketsSizing(8*8*8*8, 3, 4, size_index, numBuckets);
		EXPECT_EQ(size_index, 2);
		EXPECT_EQ(numBuckets, 8*8*8);
	}

}

TEST(o1_hash_sizing_strategy, sizeIndex) {

	o1::hash::sizing_strategy strategy(3, 1e6);

	EXPECT_EQ(strategy.sizeIndex(0, 0), 0);
	EXPECT_EQ(strategy.sizeIndex(0, 8), 0);
	EXPECT_EQ(strategy.sizeIndex(0, 8 + 1), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 8 + 1), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 2 * 8), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 8 * 8 + 1), 2);
	EXPECT_EQ(strategy.sizeIndex(1, 8 + 1), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 8), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 4), 1);
	EXPECT_EQ(strategy.sizeIndex(1, 3), 0);
	EXPECT_EQ(strategy.sizeIndex(10, 4), 1);
	EXPECT_EQ(strategy.sizeIndex(10, 3), 0);

}
