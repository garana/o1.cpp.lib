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


#include "o1.string.parse.hh"
#include "../errors/o1.invalid-format.hh"

uint64_t o1::xstrtoull(const char* s, int base) {
	char* end = nullptr;
	if (!isdigit(*s))
		throw InvalidFormat("unsigned integer", s);
	errno = 0;
	uint64_t ret = strtoull(s, &end, base);
	if (errno)
		throw InvalidFormat("unsigned integer", s);
	if ((*s != '\0') && (*end == '\0'))
		return ret;
	throw InvalidFormat("unsigned integer", s);
}

int64_t o1::xstrtoll(const char* s, int base) {
	char* end = nullptr;
	errno = 0;
	int64_t ret = strtoll(s, &end, base);
	if (errno)
		throw InvalidFormat("integer", s);
	if ((*s != '\0') && (*end == '\0'))
		return ret;
	throw InvalidFormat("integer", s);
}

float o1::xstrtof(const char* s) {
	char* end = nullptr;
	float ret = strtof(s, &end);
	if ((*s != '\0') && (*end == '\0'))
		return ret;
	throw InvalidFormat("float", s);
}

double o1::xstrtod(const char* s) {
	char* end = nullptr;
	double ret = strtod(s, &end);
	if ((*s != '\0') && (*end == '\0'))
		return ret;
	throw InvalidFormat("double", s);
}

long double o1::xstrtold(const char* s) {
	char* end = nullptr;
	long double ret = strtold(s, &end);
	if ((*s != '\0') && (*end == '\0'))
		return ret;
	throw InvalidFormat("long double", s);
}

bool o1::xstrtok(const char *str, const char *sep, size_t &start, size_t &stop) {
	size_t _start = start + strspn(str + start, sep);

	size_t _stop = _start + strcspn(str + _start, sep);

	if (_stop == _start)
		return false;

	start = _start;
	stop = _stop;

	return true;
}

bool o1::xmemtok(const char *mem, size_t mem_length, const char *sep, size_t &start, size_t &stop) {
	size_t _start = start + memspn(mem + start, mem_length - start, sep);

	size_t _stop = _start + memcspn(mem + _start, mem_length - _start, sep);

	if (_stop == _start)
		return false;

	start = _start;
	stop = _stop;

	return true;
}

const char *o1::skipws(const char *s) {
	while (*s && isspace(*s))
		++s;
	return s;
}

char *o1::skipws(char *s) {
	while (*s && isspace(*s))
		++s;
	return s;
}



size_t o1::memspn(const char* s, size_t max_len, const char* accept) {
	const char* c = s;

	for (; *c && (c - s < max_len); ++c) {

		const char* _accept = accept;

		for (; *_accept; ++_accept) {
			if (*c == *_accept)
				break;
		}

		if (!*_accept)
			break;

	}

	return c - s;
}

size_t o1::memcspn(const char* s, size_t max_len, const char* reject) {
	const char* c = s;

	for (; *c && (c - s < max_len); ++c) {

		const char* _reject = reject;

		for (; *_reject; ++_reject) {
			if (*c == *_reject)
				break;
		}

		if (*_reject)
			break;

	}

	return c - s;
}

std::string o1::join(const std::vector<std::string>& v, const std::string& sep) {
	std::string ret;

	bool isFirst = true;
	for (const auto& s: v) {
		if (!isFirst)
			ret += sep;
		ret += s;
		isFirst = false;
	}

	return ret;
}

std::vector<std::string> o1::split(const std::string& s, const std::string& sep) {
	std::vector<std::string> ret;

	std::string::size_type pos, prev = 0;

	while ((pos = s.find(sep, prev)) != std::string::npos) {
		ret.push_back(s.substr(prev, pos - prev));
		pos += sep.length();
		prev = pos;
	}

	ret.push_back(s.substr(prev, pos - prev));
	return ret;
}

std::vector<std::string> o1::split(const std::string& s, const std::regex& sep) {
	return {
		std::sregex_token_iterator{begin(s), end(s), sep, -1},
		std::sregex_token_iterator{}
	};
}

o1::char_class::char_class(const char* char_set) {
	for (const char* c = char_set; *c; ++c) {

		if (*c == '\\') {
			++c;
			if (!*c)
				throw o1::InvalidFormat(char_set, "char_class");
			addChar(c);
			continue;
		}

		// parse char class
		if ((*c == '-') && (c != char_set)) {
			char stt = c[-1];

			char end = *++c;
			if (!end)
				throw o1::InvalidFormat(char_set, "char_class");

			if (end < stt)
				throw o1::InvalidFormat(char_set, "char_class");

			for (char _c = stt; _c <= end; ++_c) {
				addChar(&_c);
			}

		} else {
			addChar(c);

		}

	}
}

size_t o1::char_class::accept(const char* s, size_t max_len) {
	const char* c = s;

	for (; *c && (c - s < max_len); ++c) {
		if (!hasChar(c))
			break;
	}

	return c - s;
}


size_t o1::char_class::reject(const char* s, size_t max_len) {
	const char* c = s;

	for (; *c && (c - s < max_len); ++c) {
		if (hasChar(c))
			break;
	}

	return c - s;
}

