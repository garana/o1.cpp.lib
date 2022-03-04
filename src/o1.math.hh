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


#ifndef O1CPPLIB_O1_MATH_HH
#define O1CPPLIB_O1_MATH_HH

#include <cmath>

namespace o1 {

	template <typename T>
	constexpr inline T max(T a, T b) {
		static_assert(
			std::is_arithmetic<T>::value,
			"o1::max can only be used in numeric types"
		);
		return a < b ? b : a;
	}

	template <typename T>
	constexpr inline T min(T a, T b) {
		static_assert(
			std::is_arithmetic<T>::value,
			"o1::min can only be used in numeric types"
		);
		return a < b ? a : b;
	}

	template <typename T>
	T square(T x) {
		return x * x;
	}

	/**
	 * Lower incomplete gamma function.
	 * @see https://en.wikipedia.org/wiki/Incomplete_gamma_function#Holomorphic_extension
	 * @see https://en.wikipedia.org/wiki/Incomplete_gamma_function#Evaluation_formulae
	 */
	template <typename T>
	T ligamma(T s, T x) {
		if (x < 0)
			return 0.0;

		double scale = pow(x, s) * exp(-x);

		double sum = 1 / s;
		double num = 1;
		double den = s;

		for (int i = 1; i < 200; i++) {
			num *= x;
			den *= s + i;
			sum += (num / den);
		}

		return sum * scale;
	}
}

#endif //O1CPPLIB_O1_MATH_HH
