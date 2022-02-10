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


#ifndef O1CPPLIB_O1_STRING_PARSE_HH
#define O1CPPLIB_O1_STRING_PARSE_HH

#include <cstddef>
#include <string>
#include <vector>
#include <regex>
#include "../o1.int.hh"

namespace o1 {

	uint64_t xstrtoull(const char* s, int base = 10);
	inline uint64_t xstrtoull(const std::string& s, int base = 10) { return xstrtoull(s.c_str(), base); }

	int64_t xstrtoll(const char* s, int base = 10);
	inline int64_t xstrtoll(const std::string& s, int base = 10) { return xstrtoll(s.c_str(), base); }

	float xstrtof(const char* s);
	inline float xstrtof(const std::string& s) { return xstrtof(s.c_str()); }

	double xstrtod(const char* s);
	inline double xstrtod(const std::string& s) { return xstrtod(s.c_str()); }

	long double xstrtold(const char* s);
	inline long double xstrtold(const std::string& s) { return xstrtold(s.c_str()); }


	/**
	 * strtok_r(3) alike, but works with const char* (does not modify
	 * @param str).
	 *
	 * If a token is found:
	 * - @param start and @param stop are modified to specify the offset of
	 *   the new token, which is in [str+start, str+stop-1].
	 * - true is returned.
	 *
	 * If no token is found:
	 * - @param start and @param stop are not modified.
	 * - false is returned.
	 *
	 * A typical usage this function would look like:
	 *   size_t start = 0;
	 *   size_t stop = 0; // initial value is irrelevant
	 *   while (xstrtok(str, " \t", start, stop)) {
	 *      char* token = strndup(str + start, stop - start);
	 *      // Use token, remember to free(token).
	 *      start = stop;
	 *   }
	 *
	 * @param str str+start: location of parsing start, chars in sep at the
	 *            begging are skipped.
	 * @param sep nul terminated string of separator characters.
	 * @param start if a token is found, it's set to the offset from str where
	 *              the new found token begins. in/out parameter.
	 * @param stop if a token is found, it's set to the offset from str where
	 *             the token ends (end + 1).
	 * @return true if a token is found. In that case, the new token is in:
	 *         [str+start, str+stop-1],
	 */
	bool xstrtok(const char* str, const char* sep, size_t &start, size_t &stop);


	/**
	 * strtok_r(3) alike, but works with const char* (does not modify
	 * @param mem), and does not expect the string to be nul terminated.
	 *
	 * If a token is found:
	 * - @param start and @param stop are modified to specify the offset of
	 *   the new token, which is in [mem+start, mem+stop-1].
	 * - true is returned.
	 *
	 * If no token is found:
	 * - @param start and @param stop are not modified.
	 * - false is returned.
	 *
	 * A typical usage this function would look like:
	 *   size_t start = 0;
	 *   size_t stop = 0; // initial value is irrelevant
	 *   while (xmemtok(mem, mem_length, " \t", start, stop)) {
	 *      char* token = strndup(mem + start, stop - start);
	 *      // Use token, remember to free(token).
	 *      start = stop;
	 *   }
	 *
	 * @param mem mem+start: location of parsing start, chars in sep at the
	 *            begging are skipped.
	 * @param sep nul terminated string of separator characters.
	 * @param start if a token is found, it's set to the offset from mem where
	 *              the new found token begins. in/out parameter.
	 * @param stop if a token is found, it's set to the offset from mem where
	 *             the token ends (end + 1).
	 * @return true if a token is found. In that case, the new token is in:
	 *         [mem+start, mem+stop-1],
	 */
	bool xmemtok(const char* mem, size_t str_len, const char* sep, size_t &start, size_t &stop);


	/**
	 * @see isspace(3)
	 * @return the @param s, skipping all whitespaces from the beginning.
	 */
	const char* skipws(const char* s);

	/**
	 * @see isspace(3)
	 * @return the @param s, skipping all whitespaces from the beginning.
	 */
	char* skipws(char* s);

	// str* utility functions, as mem*
	// @see https://github.com/google/google-api-cpp-client/blob/master/src/googleapis/strings/memutil.cc
	// @see charset class below for another, non-naïve implementation.
	size_t memspn(const char* s, size_t max_len, const char* accept);
	size_t memcspn(const char* s, size_t max_len, const char* reject);

	std::string join(const std::vector<std::string>& v, const std::string& sep);
	std::vector<std::string> split(const std::string& s, const std::string& sep);
	std::vector<std::string> split(const std::string& s, const std::regex& sep);

	class char_class {
		bool _set[256] = { false, };
		inline void addChar(const char* c) { _set[*reinterpret_cast<const unsigned char*>(c)] = true; }
		inline bool hasChar(const char* c) { return _set[*reinterpret_cast<const unsigned char*>(c)]; }

	public:
		/**
		 * By default, all characters are excluded from the character class.
		 *
		 * Format of @param char_set:
		 * - Any char present will be added to the char class, some characters
		 *   have a special meaning.
		 * - If a dash ('-') is to be included in the set, it has to be the first
		 *   character in @param char_set; or it has to be prefixed with a
		 *   backslash.
		 * - A backslash adds the following character to the set (could be used
		 *   to include dash ('-') or nul (\0) char.
		 * - A range of chars can be added to the class by specifying the range:
		 *   - "a-fA-F0-9": adds hexadecimal characters.
		 *   - "a-z": adds lower case letters.
		 *   - ".0-9": adds dot an all digits.
		 *   - "-a-zA-Z0-9": adds letters (lower and upper case), digits and
		 *     dash ('-').
		 *   - "a-" will make the constructor to throw an o1::InvalidFormat.
		 */
		explicit char_class(const char* char_set);

		/**
		 * memspn alike, with a cached charset.
		 * @return the length of chars from the beginning of @param str, and
		 *         up to @param max_len that are inside the character class.
		 */
		size_t accept(const char* str, size_t max_len);

		/**
		 * memcspn alike, with a cached charset.
		 * @return the length of chars from the beginning of @param str, and
		 *         up to @param max_len that are not inside the character class.
		 */
		size_t reject(const char* str, size_t max_len);
	};

}

#endif //O1CPPLIB_O1_STRING_PARSE_HH
