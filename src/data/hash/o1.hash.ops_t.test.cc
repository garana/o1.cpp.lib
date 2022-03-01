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
#include <valarray>
#include <cmath>
#include "o1.hash.ops_t.hh"
#include "../../o1.int.hh"
#include "../../o1.math.hh"

template <typename T>
static void expectHashValueEQ(T x, o1::hash::hash_val expected) {
	auto value = o1::hton<T>(x);
	EXPECT_EQ(o1::hash::hashValue(&value, sizeof(value), 0), expected)
					<< "hashValue(" << x << ")";
}

static double chiSquared(double x, double dof) {
	auto num = o1::ligamma<double>(dof / 2, x / 2);
	auto den = std::tgamma(dof / 2);
	return num / den;
}

/**
 * Test the test code.
 */
TEST(o1_chi_squared, samples) {
	EXPECT_NEAR(chiSquared(9.236, 5), .9, 1e-4);
}

/**
 * Integers are converted to network byte order to make the test
 * portable to other architectures.
 */
TEST(o1_hash_ops, hashValue) {
	// If the implementation of hashValue() changes, these can be updated.
	expectHashValueEQ<uint32_t>(0, 0);
	expectHashValueEQ<uint32_t>(1, 35937);
	expectHashValueEQ<uint32_t>(4, 143748);
	expectHashValueEQ<int32_t>(0, 0);
	expectHashValueEQ<int32_t>(1, 35937);
	expectHashValueEQ<int32_t>(4, 143748);
	expectHashValueEQ<int32_t>(-1, 9450300);
	expectHashValueEQ<int32_t>(-4, 9342489);
	expectHashValueEQ<int32_t>(-100, 5892537);

	std::vector<uint32_t> counts[] {
		std::vector<uint32_t>(8, 0),
		std::vector<uint32_t>(64, 0),
		std::vector<uint32_t>(512, 0),
		std::vector<uint32_t>(4096, 0)
	};

	for (uint32_t i = 0; i != 1000 * 1000; ++i) {
		// TODO test with & without o1::hton
		auto value = o1::hton<uint32_t>(i);
		auto hashValue = o1::hash::hashValue(&value, sizeof(value), 0);

		for (int icounterv = 0; icounterv < sizeof(counts) / sizeof(counts[0]); ++icounterv) {
			auto& counterv = counts[icounterv];
			counterv[hashValue % counterv.size()]++;
		}

		if (i % 1024 == 0) {
			// Every 1024 loops, we estimate ChiSq, on each of the counter vectors

			for (auto& counters: counts) {
				// only test if we had enough samples
				if (i > counters.size()) {
					double sum = 0;
					double expected_count =
						static_cast<double>(i) /
						static_cast<double>(counters.size());

					for (auto& counter: counters) {
						sum += o1::square(counter - expected_count) /
							   expected_count;
					}

					// degrees of freedom.
					auto dof = static_cast<double>(counters.size() - 1);
					// We allow 5% risk
					double chiSquareResult = chiSquared(.05, dof);

					// With 95% confidence, allow a probability of less than
					// 1e-6 the hashValue produces a non-uniform distribution.
					EXPECT_LT(chiSquareResult, 1e-6)
						<< "entries=" << i << ' '
						<< "size=" << counters.size() << ' '
						<< "sum=" << sum << ' '
						<< "χ2=" << chiSquareResult << "\n";
				}

			}

		}

	}

}
