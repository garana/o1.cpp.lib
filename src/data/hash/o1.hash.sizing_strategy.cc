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


#include <limits>
#include "./o1.hash.sizing_strategy.hh"
#include "./o1.hash.conf.hh"
#include "../../o1.logging.hh"

// TODO loadExponent value from hash_table; now it's a little bit inconsistent.
o1::hash::sizing_strategy::sizing_strategy() :
	_loadExponent(O1_HASH_TABLE_DEFAULT_LOAD_EXPONENT),
	_maxSizingIndex(O1_HASH_TABLE_DEFAULT_MAX_BUCKET_SIZES_COUNT - 1) {

	o1::xassert( // TODO avoid repeated code
		_loadExponent > 2,
		"sizing_strategy: loadExponent must be greater than 2"
	);

}

o1::hash::sizing_strategy::sizing_strategy(
	size_t loadExponent,
	size_t maxSize) :
	_loadExponent(loadExponent),
	_maxSizingIndex(maxSizingIndex(loadExponent, maxSize)) {

	o1::xassert( // TODO avoid repeated code
		_loadExponent > 2,
		"sizing_strategy: loadExponent must be greater than 2"
	);

}

size_t
o1::hash::sizing_strategy::maxSizingIndex(size_t loadExponent, size_t maxSize) {

	size_t sizeIndex = 0;
	size_t numBuckets = 0;

	bucketsSizing(
		maxSize,
		loadExponent,
		std::numeric_limits<size_t>::max(),
		sizeIndex,
		numBuckets
	);

	return sizeIndex;
}

void o1::hash::sizing_strategy::bucketsSizing(
	size_t numElements,
	size_t loadExponent,
	size_t maxSizeIndex,
	size_t& sizeIndex,
	size_t& numBuckets) {

	// TODO speedup? http://aggregate.org/MAGIC/#Log2%20of%20an%20Integer

	sizeIndex = 0;
	numBuckets = 1 << loadExponent;
	size_t maxElements = numBuckets << loadExponent;

	while (
		(numElements > maxElements) &&
		(sizeIndex < maxSizeIndex)
	) {
		numBuckets = maxElements;
		maxElements <<= loadExponent;
		++sizeIndex;
	}

}

size_t o1::hash::sizing_strategy::maxElements() const {
	return maxElements(_maxSizingIndex);
}

size_t o1::hash::sizing_strategy::maxElements(size_t sizeIndex) const {
	return static_cast<size_t>(1) << (_loadExponent * (sizeIndex + 1));
}

size_t o1::hash::sizing_strategy::sizeIndex(
	size_t currentSizeIndex,
	size_t numElements) const {

	if (
		(currentSizeIndex < _maxSizingIndex - 1) &&
		numElements > maxElements(currentSizeIndex)
		) {
		return ++currentSizeIndex;
	}

	// In case a massive amount of entries got detached on their own.
	while (
		currentSizeIndex > 0 &&
		numElements < maxElements(currentSizeIndex - 1) / 2
	) {
		--currentSizeIndex;
	}

	return currentSizeIndex;

}
