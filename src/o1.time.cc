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

#include <stdexcept>
#include <cstring>
#include <cinttypes>
#include <cmath>

extern "C" {
#include "sys/time.h"
}

#include "o1.time.hh"
#include "o1.string.hh"

o1::duration_t
o1::fromMS(int64_t ms) {
	return {std::chrono::milliseconds(ms)};
}

o1::duration_t
o1::fromSeconds(int64_t seconds) {
	return {std::chrono::seconds (seconds)};
}

o1::duration_t
o1::fromEpoch(time_t t) {
	return { std::chrono::seconds(t) };
}

o1::duration_t
o1::fromTimeVal(const timeval& t) {
	return {
		std::chrono::microseconds (t.tv_sec * 1000 * 1000 + t.tv_usec)
	};
}

int64_t
o1::toMS(o1::duration_t duration) {
	return duration.count() / 1000;
}

int64_t
o1::toSeconds(o1::duration_t duration) {
	return duration.count() / (1000 * 1000);
}

struct timeval
o1::toTimeVal(o1::duration_t duration) {
	return {
		.tv_sec = toSeconds(duration),
		.tv_usec = duration.count() % (1000 * 1000)
	};
}

o1::duration_t
o1::duration_aton(const std::string &s) {
	o1::duration_t retVal{0};

	const char* stt = s.c_str();

	if (!*stt)
		throw std::runtime_error("o1::duration_aton: Empty time duration.");

	int64_t sign = 0;

	while (*stt) {
		bool firstValue = stt == s.c_str();
		const char* unit = stt;
		int64_t value = strtoll(stt, (char**)&unit, 10);
		size_t offset = unit - s.c_str();

		// only the first conversion has a sign
		if (!firstValue && (value < 0))
			throw std::runtime_error(
				stringf("o1::duration_aton: "
						"only first value can have a negative sign."
						" %s at offset %zd",
					s.c_str(),
					offset)
			);

		if (firstValue)
			sign = value < 0 ? -1 : 1;

		if (!firstValue)
			value = sign * value;

		if (unit == stt) {
			throw std::runtime_error(
				o1::stringf(
					"o1::duration_aton: invalid time duration."
					" %s at offset %zd",
					s.c_str(),
					offset
				)
			);
		}

#define apply_unit(suffix, type)                                               \
		if (strncmp(unit, (suffix), sizeof((suffix)) - 1) == 0) {              \
			stt = unit + sizeof((suffix)) - 1;                                 \
			retVal += (type){value};                                           \
            continue;                                                          \
		}                                                                      \

		apply_unit("ms", std::chrono::milliseconds);
		apply_unit("us", std::chrono::microseconds);

		apply_unit("minutes", std::chrono::minutes);
		apply_unit("minute", std::chrono::minutes);
		apply_unit("min", std::chrono::minutes);
		apply_unit("m", std::chrono::minutes);

		apply_unit("hours", std::chrono::hours);
		apply_unit("hour", std::chrono::hours);
		apply_unit("hr", std::chrono::hours);
		apply_unit("h", std::chrono::hours);

		apply_unit("seconds", std::chrono::seconds);
		apply_unit("second", std::chrono::seconds);
		apply_unit("sec", std::chrono::seconds);
		apply_unit("s", std::chrono::seconds);

		// apply seconds with fraction
		unit = stt;
		long double value_float = strtold(stt, (char**)&unit);
		offset = unit - s.c_str();

		if (!firstValue)
			value_float = sign * value_float;

#define apply_unit_float(suffix, type)                                         \
		if (strncmp(unit, (suffix), sizeof((suffix)) - 1) == 0) {              \
			stt = unit + sizeof((suffix)) - 1;                                 \
			retVal += std::chrono::seconds{                                    \
				std::chrono::seconds::rep(truncl(value_float))                 \
			};                                                                 \
			retVal += std::chrono::microseconds{                               \
				std::chrono::microseconds::rep(                                \
					truncl(value_float * 1e6)                                  \
				) % 1000000                                                    \
			};                                                                 \
            continue;                                                          \
		}

		apply_unit_float("seconds", std::chrono::seconds);
		apply_unit_float("second", std::chrono::seconds);
		apply_unit_float("sec", std::chrono::seconds);
		apply_unit_float("s", std::chrono::seconds);

		throw std::runtime_error(
			stringf(
				"o1::duration_aton: "
				"unknown time units in: '%s' at offset %zd",
				s.c_str(),
				offset
			)
		);

	}

	return retVal;
}

std::string o1::duration_ntoa(o1::duration_t duration) {
	std::string result;
	bool is_negative = duration.count() < 0;
	if (is_negative)
		duration = -duration;

	auto hours =
		std::chrono::duration_cast<std::chrono::hours>(
			duration
		);

	auto minutes =
		std::chrono::duration_cast<std::chrono::minutes>(
			duration - hours
		);

	auto seconds =
		std::chrono::duration_cast<std::chrono::seconds>(
			duration - hours - minutes
		);

	auto ms =
		std::chrono::duration_cast<std::chrono::milliseconds>(
			duration - hours - minutes - seconds
		);

	auto us =
		std::chrono::duration_cast<std::chrono::microseconds>(
			duration - hours - minutes - seconds
		);

	if (hours.count() != 0)
		result = o1::stringf("%" PRId64 "h", hours.count());

	if (minutes.count() != 0)
		result += o1::stringf("%" PRId64 "m", minutes.count());

	bool useMS = (us.count() % 1000) == 0;

	switch (
		(seconds.count() != 0 ? 1 : 0) |
		(ms.count() != 0 ? 2 : 0) |
		(us.count() != 0 ? 4 : 0)
	) {

		case 0: // all are zero
			break;

		case 1: // sec!=0, ms=0, us=0
			result += o1::stringf("%" PRId64 "s", seconds.count());
			break;

		case 2: // sec=0, ms!=0, us=0
			if (result.length() != 0)
				result += o1::stringf("0.%03" PRId64 "s", ms.count());
			else
				result = o1::stringf("%" PRId64 "ms", ms.count());
			break;

		case 3: // sec!=0, ms!=0, us=0
			result += o1::stringf("%" PRId64 ".%03" PRId64 "s", seconds.count(), ms.count());
			break;
			
		case 4: // sec=0, ms!=0, us!=0
			if (result.length() != 0) {
				if (useMS) {
					result += o1::stringf("0.%03" PRId64 "s", ms.count());
				} else {
					result += o1::stringf("0.%06" PRId64 "s", us.count());
				}
			} else {
				if (useMS) {
					result = o1::stringf("%" PRId64 "ms", ms.count());
				} else {
					result = o1::stringf("%" PRId64 "us", us.count());
				}
			}
			break;

		case 5: // sec!=0, ms=0, us!=0
			result += o1::stringf("%" PRId64 ".%06" PRId64 "s", seconds.count(), us.count());
			break;

		case 6: // sec=0, ms!=0, us!=0
			if (result.length() != 0) {
				if (useMS) {
					result += o1::stringf("0.%03" PRId64 "s", ms.count());
				} else {
					result += o1::stringf("0.%06" PRId64 "s", us.count());
				}
			} else {
				if (useMS) {
					result = o1::stringf("%" PRId64 "ms", ms.count());
				} else {
					result = o1::stringf("%" PRId64 "us", us.count());
				}
			}
			break;

		case 7: // sec!=0, ms!=0, us!=0
			if (useMS) {
				result += o1::stringf("%" PRId64 ".%03" PRId64 "s", seconds.count(), ms.count());
			} else {
				result += o1::stringf("%" PRId64 ".%06" PRId64 "s", seconds.count(), us.count());
			}
			break;

	}

	if (is_negative)
		return "-" + result;

	return result;
}

template<>
o1::duration_t o1::aton<o1::duration_t>(const std::string& s) { return duration_aton(s); }

template<>
std::string o1::ntoa<o1::duration_t>(o1::duration_t t) { return duration_ntoa(t); }

o1::wall_clock_t o1::now() {
	return std::chrono::time_point_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now()
	);
}

bool operator == (const timeval& left, const timeval& right) {
	return timercmp(&left, &right, !=) == 0;
}
