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
#include "o1.time.hh"

namespace {

	TEST(o1_time, fromMS) {
		EXPECT_EQ(o1::fromMS(0).count(), 0);
		EXPECT_EQ(o1::fromMS(1).count(), 1000);
		EXPECT_EQ(o1::fromMS(1000 * 1000).count(), 1000 * 1000 * 1000);
	}

	TEST(o1_time, fromSeconds) {
		EXPECT_EQ(o1::fromSeconds(0).count(), 0);
		EXPECT_EQ(o1::fromSeconds(1).count(), o1::DURATION_1sec);
		EXPECT_EQ(o1::fromSeconds(1000 * 1000).count(), 1000 * 1000 * o1::DURATION_1sec);
	}

	TEST(o1_time, fromEpoch) {
		EXPECT_EQ(o1::fromEpoch(0).count(), 0);
		EXPECT_EQ(o1::fromEpoch(1).count(), 1000 * 1000);
		EXPECT_EQ(o1::fromEpoch(1000 * 1000).count(), 1000 * 1000 * o1::DURATION_1sec);
	}

	TEST(o1_time, fromTimeVal) {
		EXPECT_EQ(o1::fromTimeVal(timeval{ .tv_sec = 0, .tv_usec = 0 }).count(), 0);
		EXPECT_EQ(o1::fromTimeVal(timeval{ .tv_sec = 1, .tv_usec = 0 }).count(), o1::DURATION_1sec);
		EXPECT_EQ(o1::fromTimeVal(timeval{ .tv_sec = 1, .tv_usec = 100 }).count(), o1::DURATION_1sec + 100 * o1::DURATION_1us);
		EXPECT_EQ(o1::fromTimeVal(timeval{ .tv_sec = 15, .tv_usec = 100 }).count(), 15 * o1::DURATION_1sec + 100 * o1::DURATION_1us);
	}

	TEST(o1_time, toMS) {
		EXPECT_EQ(o1::toMS(o1::duration_t{0}), 0);
		EXPECT_EQ(o1::toMS(o1::duration_t{1}), 0);
		EXPECT_EQ(o1::toMS(o1::duration_t{1000}), 1);
		EXPECT_EQ(o1::toMS(o1::duration_t{1000 * 1000}), 1000);
	}

	TEST(o1_time, toSeconds) {
		EXPECT_EQ(o1::toSeconds(o1::duration_t{0}), 0);
		EXPECT_EQ(o1::toSeconds(o1::duration_t{1}), 0);
		EXPECT_EQ(o1::toSeconds(o1::duration_t{1000 * 1000}), 1);
	}

	TEST(o1_time, toTimeVal) {
		const auto zero = timeval{ .tv_sec = 0, .tv_usec = 0 };
		const auto _1_micro_second = timeval{ .tv_sec = 0, .tv_usec = 1 };
		const auto _700_milli_seconds = timeval{ .tv_sec = 0, .tv_usec = 7000 };
		const auto _1_second = timeval{ .tv_sec = 1, .tv_usec = 0 };
		const auto _1M_seconds = timeval{ .tv_sec = 1000 * 1000, .tv_usec = 0 };

		EXPECT_EQ(o1::toTimeVal(o1::duration_t{0}), zero);
		EXPECT_EQ(o1::toTimeVal(o1::duration_t{o1::DURATION_1us}), _1_micro_second);
		EXPECT_EQ(o1::toTimeVal(o1::duration_t{7 * o1::DURATION_1ms}), _700_milli_seconds);
		EXPECT_EQ(o1::toTimeVal(o1::duration_t{o1::DURATION_1sec}), _1_second);
		EXPECT_EQ(o1::toTimeVal(o1::duration_t{1000 * 1000 * o1::DURATION_1sec}), _1M_seconds);
	}

	TEST(o1_time, duration_aton) {

		EXPECT_EQ(o1::duration_aton("0us").count(), 0 * o1::DURATION_1us);
		EXPECT_EQ(o1::duration_aton("1us").count(), 1 * o1::DURATION_1us);
		EXPECT_EQ(o1::duration_aton("9us").count(), 9 * o1::DURATION_1us);
		EXPECT_EQ(o1::duration_aton("-9us").count(), -9 * o1::DURATION_1us);

		EXPECT_EQ(o1::duration_aton("0ms").count(), 0 * o1::DURATION_1ms);
		EXPECT_EQ(o1::duration_aton("1ms").count(), 1 * o1::DURATION_1ms);
		EXPECT_EQ(o1::duration_aton("9ms").count(), 9 * o1::DURATION_1ms);
		EXPECT_EQ(o1::duration_aton("-9ms").count(), -9 * o1::DURATION_1ms);

		EXPECT_EQ(o1::duration_aton("0s").count(), 0 * o1::DURATION_1sec);
		EXPECT_EQ(o1::duration_aton("1s").count(), 1 * o1::DURATION_1sec);
		EXPECT_EQ(o1::duration_aton("9s").count(), 9 * o1::DURATION_1sec);
		EXPECT_EQ(o1::duration_aton("-9s").count(), -9 * o1::DURATION_1sec);

		EXPECT_EQ(o1::duration_aton("0m").count(), 0 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("1m").count(), 1 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("9m").count(), 9 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("-9m").count(), -9 * o1::DURATION_1min);

		EXPECT_EQ(o1::duration_aton("0min").count(), 0 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("1min").count(), 1 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("9min").count(), 9 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("-9min").count(), -9 * o1::DURATION_1min);

		EXPECT_EQ(o1::duration_aton("0minute").count(), 0 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("1minute").count(), 1 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("9minute").count(), 9 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("-9minute").count(), -9 * o1::DURATION_1min);

		EXPECT_EQ(o1::duration_aton("0minutes").count(), 0 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("1minutes").count(), 1 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("9minutes").count(), 9 * o1::DURATION_1min);
		EXPECT_EQ(o1::duration_aton("-9minutes").count(), -9 * o1::DURATION_1min);

		EXPECT_EQ(o1::duration_aton("-1h2m").count(),
			-1 * (
					 1 * o1::DURATION_1hour +
					 2 * o1::DURATION_1min
				 )
		);

		EXPECT_EQ(o1::duration_aton("-1m2.005sec").count(),
			-1 * (
					 1 * o1::DURATION_1min +
					 2 * o1::DURATION_1sec +
					 5 * o1::DURATION_1ms
				 )
		);

		EXPECT_EQ(o1::duration_aton("-1h3m12.005sec").count(),
			-1 * (
					 o1::DURATION_1hour +
					 3 * o1::DURATION_1min +
					 12 * o1::DURATION_1sec +
					 5 * o1::DURATION_1ms
				 )
		);
	}

	TEST(o1_time, duration_ntoa) {
		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{o1::DURATION_1us}), "1us");
		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{o1::DURATION_1ms}), "1ms");
		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{o1::DURATION_1sec}), "1s");
		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{o1::DURATION_1min}), "1m");
		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{o1::DURATION_1hour}), "1h");

		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{
			o1::DURATION_1hour +
			o1::DURATION_1min +
			o1::DURATION_1sec +
			o1::DURATION_1us
		}), "1h1m1.000001s");

		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{
			5 *o1::DURATION_1hour - o1::DURATION_1us
		}), "4h59m59.999999s");

		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{
			-1 * (
					 o1::DURATION_1hour +
					 o1::DURATION_1min +
					 o1::DURATION_1sec +
					 o1::DURATION_1us
				 )
		}), "-1h1m1.000001s");

		EXPECT_EQ(o1::duration_ntoa(o1::duration_t{
			-1 * (
				2 * o1::DURATION_1hour +
				5 * o1::DURATION_1min +
				1 * o1::DURATION_1sec +
				1 * o1::DURATION_1us
			)
		}), "-2h5m1.000001s");

	}
}