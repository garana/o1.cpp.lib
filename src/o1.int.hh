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


#ifndef O1CPPLIB_O1_INT_HH
#define O1CPPLIB_O1_INT_HH

#include <cstdint>

typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

namespace o1 {

	/**
	 * Network to host byte order.
	 * @tparam T integral type to use ([u]int{8,16,32,64}).
	 * @param x value in network byte order.
	 * @return value in host byte order.
	 */
	template <typename T> T ntoh(T x);

	/**
	 * Host to network byte order.
	 * @tparam T integral type to use ([u]int{8,16,32,64}).
	 * @param x value in network byte order.
	 * @return value in host byte order.
	 */
	template <typename T> T hton(T x);

	template <> uint8_t ntoh<uint8_t>  (uint8_t x);
	template <> uint16_t ntoh<uint16_t> (uint16_t x);
	template <> uint32_t ntoh<uint32_t> (uint32_t x);
	template <> uint64_t ntoh<uint64_t> (uint64_t x);

	template <> int8_t ntoh<int8_t>   (int8_t x);
	template <> int16_t ntoh<int16_t>  (int16_t x);
	template <> int32_t ntoh<int32_t>  (int32_t x);
	template <> int64_t ntoh<int64_t>  (int64_t x);

	template <> uint8_t hton<uint8_t>  (uint8_t x);
	template <> uint16_t hton<uint16_t> (uint16_t x);
	template <> uint32_t hton<uint32_t> (uint32_t x);
	template <> uint64_t hton<uint64_t> (uint64_t x);

	template <> int8_t hton<int8_t>   (int8_t x);
	template <> int16_t hton<int16_t>  (int16_t x);
	template <> int32_t hton<int32_t>  (int32_t x);
	template <> int64_t hton<int64_t>  (int64_t x);

}

#endif //O1CPPLIB_O1_INT_HH
