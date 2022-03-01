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
#include "o1.flag_set.hh"

enum class enumSmallTest {
	enNONE = 0,
	enOne = 1,
	enTwo = 2,
	en31 = 31
};

enum class enum64Test {
	enNONE = 0,
	enOne = 1,
	enTwo = 2,
	en31 = 31,
	en63 = 63
};

enum class enumLargeTest {
	enNONE = 0,
	enOne = 1,
	enTwo = 2,
	en31 = 31,
	en63 = 63,
	en127 = 127,
};

TEST(flag_set, small) {
	o1::flag_set<enumSmallTest> test_set;
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), false);

	test_set.set(enumSmallTest::enOne);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), false);

	test_set.set(enumSmallTest::enTwo);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), false);

	test_set.clear(enumSmallTest::enOne);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), false);

	test_set.set(enumSmallTest::en31);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), true);

	test_set.clear(enumSmallTest::en31);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumSmallTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumSmallTest::en31), false);

}

TEST(flag_set, sixtyFourBits) {
	o1::flag_set<enum64Test, uint64_t> test_set;
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.set(enum64Test::enOne);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), true);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.set(enum64Test::enTwo);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), true);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.clear(enum64Test::enOne);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.set(enum64Test::en31);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.clear(enum64Test::en31);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);

	test_set.set(enum64Test::en63);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), true);

	test_set.clear(enum64Test::en63);
	EXPECT_EQ(test_set.isSet(enum64Test::enNONE), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enOne), false);
	EXPECT_EQ(test_set.isSet(enum64Test::enTwo), true);
	EXPECT_EQ(test_set.isSet(enum64Test::en31), false);
	EXPECT_EQ(test_set.isSet(enum64Test::en63), false);
}

TEST(flag_set, large) {

	o1::flag_set<enumLargeTest, std::vector<bool>> test_set;
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.set(enumLargeTest::enOne);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.set(enumLargeTest::enTwo);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.clear(enumLargeTest::enOne);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.set(enumLargeTest::en31);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.clear(enumLargeTest::en31);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.set(enumLargeTest::en63);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.clear(enumLargeTest::en63);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);

	test_set.set(enumLargeTest::en127);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), true);

	test_set.clear(enumLargeTest::en127);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enNONE), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enOne), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::enTwo), true);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en31), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en63), false);
	EXPECT_EQ(test_set.isSet(enumLargeTest::en127), false);
}
