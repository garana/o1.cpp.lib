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


#ifndef O1CPPLIB_O1_STRING_CONVERSION_HH
#define O1CPPLIB_O1_STRING_CONVERSION_HH

#include <string>
#include "../o1.int.hh"

namespace o1 {

	/**
	 * Convert a numeric value into a base 10 string representation.
	 * @tparam T Numeric Type of the value to be converted.
	 * @param value Value to be converted.
	 * @return string representation in base 10.
	 */
	template <typename T> std::string ntoa(T value); // TODO write UTs
	template <> std::string ntoa<int8_t>(int8_t value);
	template <> std::string ntoa<int16_t>(int16_t value);
	template <> std::string ntoa<int32_t>(int32_t value);
	template <> std::string ntoa<int64_t>(int64_t value);
	template <> std::string ntoa<int128_t>(int128_t value);
	template <> std::string ntoa<uint8_t>(uint8_t value);
	template <> std::string ntoa<uint16_t>(uint16_t value);
	template <> std::string ntoa<uint32_t>(uint32_t value);
	template <> std::string ntoa<uint64_t>(uint64_t value);
	template <> std::string ntoa<uint128_t>(uint128_t value);
	template <> std::string ntoa<float>(float value);
	template <> std::string ntoa<double>(double value);
	template <> std::string ntoa<long double>(long double value);

	/**
	 * Convert a string into a numeric value.
	 * @tparam T type of the returned value.
	 * @param s string containing the representation.
	 *          For integral types:
	 *          - If it begins with "0x" or "0X", it's assumed to be in base 16.
	 *          - If it begins with "0", it's assumed to be in base 8.
	 *          - Base 10 is assumed otherwise.
	 *          For floating point types, always base 10 is assumed.
	 * @return
	 */
	template <typename T> T aton(const std::string& s);
	template <> int8_t aton<int8_t>(const std::string& s);
	template <> int16_t aton<int16_t>(const std::string& s);
	template <> int32_t aton<int32_t>(const std::string& s);
	template <> int64_t aton<int64_t>(const std::string& s);
	template <> int128_t aton<int128_t>(const std::string& s);
	template <> uint8_t aton<uint8_t>(const std::string& s);
	template <> uint16_t aton<uint16_t>(const std::string& s);
	template <> uint32_t aton<uint32_t>(const std::string& s);
	template <> uint64_t aton<uint64_t>(const std::string& s);
	template <> uint128_t aton<uint128_t>(const std::string& s);
	template <> float aton<float>(const std::string& s);
	template <> double aton<double>(const std::string& s);
	template <> long double aton<long double>(const std::string& s);

}

#endif //O1CPPLIB_O1_STRING_CONVERSION_HH
