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


#include "o1.int.hh"
#include <endian.h>

template <> uint8_t o1::ntoh<uint8_t>   (uint8_t x)  { return x; }
template <> uint16_t o1::ntoh<uint16_t> (uint16_t x) { return be16toh(x); }
template <> uint32_t o1::ntoh<uint32_t> (uint32_t x) { return be32toh(x); }
template <> uint64_t o1::ntoh<uint64_t> (uint64_t x) { return be64toh(x); }

template <> int8_t o1::ntoh<int8_t>    (int8_t x)  { return x; }
template <> int16_t o1::ntoh<int16_t>  (int16_t x) { return be16toh(x); }
template <> int32_t o1::ntoh<int32_t>  (int32_t x) { return be32toh(x); }
template <> int64_t o1::ntoh<int64_t>  (int64_t x) { return be64toh(x); }

template <> uint8_t o1::hton<uint8_t>   (uint8_t x)  { return x; }
template <> uint16_t o1::hton<uint16_t> (uint16_t x) { return htobe16(x); }
template <> uint32_t o1::hton<uint32_t> (uint32_t x) { return htobe32(x); }
template <> uint64_t o1::hton<uint64_t> (uint64_t x) { return htobe64(x); }

template <> int8_t o1::hton<int8_t>    (int8_t x)  { return x; }
template <> int16_t o1::hton<int16_t>  (int16_t x) { return htobe16(x); }
template <> int32_t o1::hton<int32_t>  (int32_t x) { return htobe32(x); }
template <> int64_t o1::hton<int64_t>  (int64_t x) { return htobe64(x); }
