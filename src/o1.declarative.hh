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


#ifndef O1CPPLIB_O1_DECLARATIVE_HH
#define O1CPPLIB_O1_DECLARATIVE_HH

#include <vector>

namespace o1 {

	/**
	 * Return the first non-falsy value in @param v; or @param default_value if
	 * they none of them satisfy the condition.
	 */
	template <typename T>
	T coalesce(const std::vector<T>& v, T default_value) {
		for (const T& value: v) {
			if (value)
				return value;
		}
		return default_value;
	}

	/**
	 * Return the first non-falsy value in @param v; or @param default_value if
	 * they none of them satisfy the condition.
	 */
	template <typename T>
	T coalesce(const std::vector<T>& v) {
		for (const T& value: v) {
			if (value)
				return value;
		}
		return *v.rbegin();
	}

	template <typename T> T coalesce(T value) {
		return value;
	}

	template <typename T> T coalesce(
		T value1,
		T value2
	) {
		return value1 ? value1 : value2;
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4,
		T value5
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4, value5);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4,
		T value5,
		T value6
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4, value5, value6);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4,
		T value5,
		T value6,
		T value7
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4, value5, value6, value7);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4,
		T value5,
		T value6,
		T value7,
		T value8
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4, value5, value6, value8);
	}

	template <typename T> T coalesce(
		T value1,
		T value2,
		T value3,
		T value4,
		T value5,
		T value6,
		T value7,
		T value8,
		T value9
	) {
		return value1 ?
			   value1 :
			   coalesce(value2, value3, value4, value5, value6, value7, value8, value9);
	}

}

#endif //O1CPPLIB_O1_DECLARATIVE_HH
