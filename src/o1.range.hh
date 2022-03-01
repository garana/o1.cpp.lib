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

		bool isInside(T value) { return (_start <= value) && (value < _stop); }

		bool overlaps(const range<T>& that) { // TODO write UTs
			return isInside(that._start) || isInside(that._stop);
		}

		struct LT { // TODO review, write UTs
			bool operator() (const range<T>& left, const range<T>& right) const {
				return (left._stop <= right._start);
//				return (left._start < right._start);
			}
		};

	};

}

#endif //O1CPPLIB_O1_RANGE_HH
