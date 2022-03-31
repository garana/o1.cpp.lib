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
#include "./o1.range.hh"

namespace {

	const o1::range<double> _12(1, 2);
	const o1::range<double> _10_20(10, 20);

	using drange = o1::range<double>;

	TEST(o1_range, contains) {
		EXPECT_EQ(_12.contains(0), false);
		EXPECT_EQ(_12.contains(1), true);
		EXPECT_EQ(_12.contains(1.5), true);
		EXPECT_EQ(_12.contains(2), true);
		EXPECT_EQ(_12.contains(3), false);
	}

	TEST(o1_range, isInside) {
		EXPECT_EQ(_12.isInside(0), false);
		EXPECT_EQ(_12.isInside(1), false);
		EXPECT_EQ(_12.isInside(1.5), true);
		EXPECT_EQ(_12.isInside(2), false);
		EXPECT_EQ(_12.isInside(3), false);
	}

	TEST(o1_range, clamp) {
		EXPECT_EQ(_12.clamp(0), 1);
		EXPECT_EQ(_12.clamp(1), 1);
		EXPECT_EQ(_12.clamp(1.5), 1.5);
		EXPECT_EQ(_12.clamp(1.5), 1.5);
		EXPECT_EQ(_12.clamp(2), 2);
		EXPECT_EQ(_12.clamp(2.5), 2);
	}

	TEST(o1_range, to01) {
		EXPECT_EQ(_10_20.to01(10), 0);
		EXPECT_EQ(_10_20.to01(15), 0.5);
		EXPECT_EQ(_10_20.to01(20), 1);
	}

	TEST(o1_range, from01) {
		EXPECT_EQ(_10_20.from01(0), 10);
		EXPECT_EQ(_10_20.from01(.5), 15);
		EXPECT_EQ(_10_20.from01(1), 20);
	}

	TEST(o1_range, map) {
		EXPECT_EQ(_10_20.map(_12, 10), 1);
		EXPECT_EQ(_10_20.map(_12, 15), 1.5);
		EXPECT_EQ(_10_20.map(_12, 20), 2);
	}

	TEST(o1_range, map0) {
		EXPECT_EQ(_10_20.map(2, 10), 0);
		EXPECT_EQ(_10_20.map(2, 15), 1);
		EXPECT_EQ(_10_20.map(2, 20), 2);
	}

	TEST(o1_range, overlaps) {
		EXPECT_EQ(drange(0,1).overlaps(drange(.5,2)), true);
		EXPECT_EQ(drange(0,1).overlaps(drange(1,2)), false);
		EXPECT_EQ(drange(0,1).overlaps(drange(2,3)), false);
	}

}