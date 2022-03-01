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


#include "o1.string.conversion.hh"
#include "o1.string.format.hh"
#include "o1.string.parse.hh"
#include "../errors/o1.invalid-format.hh"

#include <cinttypes>
#include <cfloat>

template <> std::string o1::ntoa<int8_t>(int8_t v) { return stringf("%" PRId8, v); }
template <> std::string o1::ntoa<int16_t>(int16_t v) { return stringf("%" PRId16, v); }
template <> std::string o1::ntoa<int32_t>(int32_t v) { return stringf("%" PRId32, v); }
template <> std::string o1::ntoa<int64_t>(int64_t v) { return stringf("%" PRId64, v); }

template <> std::string o1::ntoa<uint8_t>(uint8_t v) { return stringf("%" PRIu8, v); }
template <> std::string o1::ntoa<uint16_t>(uint16_t v) { return stringf("%" PRIu16, v); }
template <> std::string o1::ntoa<uint32_t>(uint32_t v) { return stringf("%" PRIu32, v); }
template <> std::string o1::ntoa<uint64_t>(uint64_t v) { return stringf("%" PRIu64, v); }

/**
 * Converts a non-negative integral value into a string representation
 */
template <typename T>
std::string naive_ntoa(T value) {

	if (!value)
		return "0";

	std::string ret;
	ret.reserve(sizeof("340282366920938463463374607431768211456"));
	int index = sizeof("340282366920938463463374607431768211456");
	while (value > 0) {
		ret[--index] = '0' + value % 10;
		value /= 10;
	}

	return ret.substr(index);
}

template <> std::string o1::ntoa<int128_t>(int128_t v) {
	return (v < 0 ? "-" : "") +
		naive_ntoa<int128_t>(v < 0 ? -v : v);
}

template <> std::string o1::ntoa<uint128_t>(uint128_t v) {
	return naive_ntoa<uint128_t>(v);
}

template <> std::string o1::ntoa<float>(float v) { return stringf("%.*g", FLT_DIG, v); }
template <> std::string o1::ntoa<double>(double v) { return stringf("%.*g", DBL_DIG, v); }
template <> std::string o1::ntoa<long double>(long double v) { return stringf("%.*Lg", LDBL_DIG, v); }

template<>
std::string o1::ntoa<std::string>(std::string v) { return v; }

/**
 * aton conversions: string to type
 */

template <typename retType, typename bigType>
retType aton_impl(const std::string& s, const std::string& bigTypeName) {
	bigType retBig = o1::aton<bigType>(s);
	retType ret = retBig;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
	if (ret != retBig)
		throw o1::InvalidFormat(bigTypeName, s);
#pragma clang diagnostic pop

	return ret;
}

template <>
int8_t o1::aton<int8_t>(const std::string& s) {
	return aton_impl<int8_t, int64_t>(s, "int8_t");
}


template <>
int16_t o1::aton<int16_t>(const std::string& s) {
	return aton_impl<int16_t, int64_t>(s, "int16_t");
}

template <>
int32_t o1::aton<int32_t>(const std::string& s) {
	return aton_impl<int32_t, int64_t>(s, "int32_t");
}

template <>
int64_t o1::aton<int64_t>(const std::string& s) {
	return o1::xstrtoll(s);
}

//template <>
//int128_t o1::xstrto<int128_t>(const std::string& s);

template <>
uint8_t o1::aton<uint8_t>(const std::string& s) {
	return aton_impl<uint8_t, uint64_t>(s, "uint8_t");
}

template <>
uint16_t o1::aton<uint16_t>(const std::string& s) {
	return aton_impl<uint16_t, uint64_t>(s, "uint16_t");
}

template <>
uint32_t o1::aton<uint32_t>(const std::string& s) {
	return aton_impl<uint32_t, uint64_t>(s, "uint32_t");
}

template <>
uint64_t o1::aton<uint64_t>(const std::string& s) {
	return o1::xstrtoull(s);
}

//template <>
//uint128_t o1::aton<uint128_t>(const std::string& s) {
//
//}

template <>
float o1::aton<float>(const std::string& s) { return xstrtof(s); }

template <>
double o1::aton<double>(const std::string& s) { return xstrtod(s); }

template <>
long double o1::aton<long double>(const std::string& s) { return xstrtold(s); }

template <>
bool o1::aton<bool>(const std::string& s) {
	if (s == "true") return true;
	if (s == "false") return false;
	throw InvalidFormat("bool", s);
}

template <>
std::string o1::aton<std::string>(const std::string& s) { return s; }
