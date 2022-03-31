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


#ifndef O1CPPLIB_O1_RANGE_HH
#define O1CPPLIB_O1_RANGE_HH

#include "./o1.property.hh"
#include "./o1.logging.hh"

namespace o1 {

	/**
	 * Defines a range [from,to)
	 * @tparam T
	 */
	template <typename T>
	class range {
		static_assert(
			std::is_arithmetic<T>::value,
			"o1::range: type must be a scalar"
		);

		/**
		 * Lower (included) bound.
		 */
		O1_READWRITE_SCALAR_PROPERTY(T, start, 0);

		/**
		 * Upper (excluded) bound.
		 */
		O1_READWRITE_SCALAR_PROPERTY(T, stop, 0);

	public:
		range(T start, T stop): _start(start), _stop(stop) {
			o1::xassert(start != stop, "o1::range: cannot be empty");
			o1::xassert(start < stop, "o1::range: values inverted?");
		}

		T span() const { return _stop - _start; }

		bool contains(T value) const {
			return (_start <= value) && (value <= _stop);
		}

		bool isInside(T value) const {
			return (_start < value) && (value < _stop);
		}

		/**
		 * Return "x", forcing it to be inside [start, stop]:
		 * - If it's smaller than start, returns start.
		 * - If it's bigger than stop, returns stop.
		 */
		T clamp(T x) const { return std::max(std::min(x, _stop), _start); }

		/**
		 * Given x, return the mapped value in [0,1].
		 */
		double to01(T x) const { return (x - _start) / (_stop - _start); }

		/**
		 * Given x in [0,1], return the value in the range.
		 * @param x
		 * @return
		 */
		T from01(double x) const { return x * (_stop - _start) + _start; }

		/**
		 * Map the value x into the new range [min,max]
		 * @param that the new range, where x is mapped into
		 * @param x value to map
		 * @return corresponding value in "that" range.
		 */
		T map(const range<T>& that, T x) const {
			return that.from01(to01(x));
		}

		/**
		 * Map the value x into the new range [0,max]
		 * @param that the new range, where x is mapped into
		 * @param x value to map
		 * @return corresponding value in "that" range.
		 */
		T map(T max, T x) const {
			return to01(x) * max;
		}

		bool overlaps(const range<T>& that) const {
			return isInside(that._start) || isInside(that._stop);
		}

	};

}

#endif //O1CPPLIB_O1_RANGE_HH
