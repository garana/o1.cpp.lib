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


#ifndef O1CPPLIB_O1_HASH_SIZING_STRATEGY_HH
#define O1CPPLIB_O1_HASH_SIZING_STRATEGY_HH

#include <cstddef>

namespace o1 {

	namespace hash {

		/**
		 * Hash table sizing math.
		 *
		 * We call loadFactor to the average number of elements per bucket
		 * (open chaining).
		 *
		 * When the loadFactor goes above a certain threshold, we'll use
		 * the next sizeIndex.
		 *
		 * Instead of specifying a loadFactor, we use the loadExponent:
		 * loadFactor = 2 ** loadExponent;
		 *
		 * We use the next sizeIndex when the loadFactor for the current
		 * sizeIndex is above the wanted value (2 ** loadExponent).
		 *
		 * We use the smaller sizeIndex when the loadFactor goes below
		 * the half of the minimum number of elements for this sizeIndex.
		 */
		class sizing_strategy {
		protected:
			/**
			 * loadFactor = numElements/bucketCount
			 * loadFactor = 2 ** _loadExponent
			 * buckets[sizeIndex] = (sizeIndex+1) ** loadFactor
			 * maxElements[sizeIndex] = buckets[sizeIndex] * loadFactor;
			 * minElements[sizeIndex] = maxElements[sizeIndex-1] / 2;
			 */
			size_t _loadExponent;
			size_t _maxSizingIndex;

		public:

			sizing_strategy();

			sizing_strategy(
				size_t loadExponent,
				size_t maxSize
			);

			inline size_t loadExponent() const { return _loadExponent; }

			inline size_t maxSizingIndex() const { return _maxSizingIndex; }

			inline size_t numBuckets(size_t sizeIndex) const {
				return static_cast<size_t>(1) << ((sizeIndex+1) * _loadExponent);
			}

			/**
			 * Maximum number of elements w/out going above the load factor.
			 */
			size_t maxElements() const;

			/**
			 * Maximum number of elements for the sizeIndex w/out going above
			 * the load factor.
			 */
			size_t maxElements(size_t sizeIndex) const;

			size_t sizeIndex(size_t currentSizeIndex, size_t numElements) const;

			static size_t maxSizingIndex(
				size_t loadExponent,
				size_t maxSize
			);

			/**
			 * Given a number of elements, return the appropriate sizeIndex
			 * and number of buckets in this sizeIndex.
			 */
			static void bucketsSizing(
				size_t numElements,
				size_t loadExponent,
				size_t maxSizeIndex,
				size_t& sizeIndex,
				size_t& numBuckets
			);

		};

	}

}

#endif //O1CPPLIB_O1_HASH_SIZING_STRATEGY_HH
