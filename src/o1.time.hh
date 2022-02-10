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


#ifndef O1CPPLIB_O1_TIME_HH
#define O1CPPLIB_O1_TIME_HH

#include <chrono>
#include <string>
#include "./string/o1.string.conversion.hh"

namespace o1 {

	/** Time is measured in microseconds */
	typedef std::chrono::microseconds duration_t;

	duration_t fromMS(int64_t ms);
	duration_t fromSeconds(int64_t seconds);
	duration_t fromEpoch(time_t t);
	duration_t fromTimeVal(const struct timeval &t);

	int64_t toMS(duration_t duration);
	int64_t toSeconds(duration_t duration);
	time_t toEpoch(duration_t duration);
	struct timeval toTimeVal(duration_t duration);

	typedef std::chrono::time_point<
		std::chrono::system_clock,
		std::chrono::microseconds
	>
		wall_clock_t;

	wall_clock_t now();

	typedef std::chrono::time_point<
		std::chrono::steady_clock,
		std::chrono::microseconds
	>
		ticker_clock_t;

	const auto ticker_clock_max =
		std::chrono::time_point<
			std::chrono::steady_clock,
			std::chrono::microseconds
		>::max();

	/**
	 * Convert a "$value$unit" string into duration_t (microseconds).
	 * If no unit is present, seconds is assumed.
	 * @param s
	 * @return
	 */
	duration_t duration_aton(const std::string& s);
	std::string duration_ntoa(duration_t);
	template <> duration_t aton<duration_t>(const std::string& s);
	template <> std::string ntoa<duration_t>(duration_t t);

	static const int64_t DURATION_1us   {1};
	static const int64_t DURATION_1ms   {1000ll};
	static const int64_t DURATION_1sec  {1000ll * 1000ll};
	static const int64_t DURATION_1min  {1000ll * 1000ll * 60ll};
	static const int64_t DURATION_1hour {1000ll * 1000ll * 60ll * 60ll};
}

bool operator == (const struct timeval& left, const struct timeval& right);

#endif //O1CPPLIB_O1_TIME_HH
