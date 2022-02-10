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

#include <gtest/gtest.h>
#include "o1.string.parse.hh"

TEST(o1_string_parse, xstrtok) {

	size_t start = 0;
	size_t end = 0;
	const char* line = "hi \tthere!\n";

	bool call1 = o1::xstrtok(line, " \t", start, end);
	EXPECT_EQ(call1, true);
	EXPECT_EQ(start, 0);
	EXPECT_EQ(end, 2);
	start = end;

	bool call2 = o1::xstrtok(line, " \t", start, end);
	EXPECT_EQ(call2, true);
	EXPECT_EQ(start, 4);
	EXPECT_EQ(end, 11);
	start = end;

	bool call3 = o1::xstrtok(line, " \t", start, end);
	EXPECT_EQ(call3, false);
	EXPECT_EQ(start, 11);
	EXPECT_EQ(end, 11);

}

TEST(o1_string_parse, xmemtok) {

	size_t start = 0;
	size_t end = 0;
	const char* line = "hi \tthere!\n";

	bool call1 = o1::xmemtok(line, strlen(line), " \t", start, end);
	EXPECT_EQ(call1, true);
	EXPECT_EQ(start, 0);
	EXPECT_EQ(end, 2);
	start = end;

	bool call2 = o1::xmemtok(line, strlen(line), " \t", start, end);
	EXPECT_EQ(call2, true);
	EXPECT_EQ(start, 4);
	EXPECT_EQ(end, 11);
	start = end;

	bool call3 = o1::xmemtok(line, strlen(line), " \t", start, end);
	EXPECT_EQ(call3, false);
	EXPECT_EQ(start, 11);
	EXPECT_EQ(end, 11);

}

TEST(o1_string_parse, skipws_const) {
	const char* result1 = o1::skipws("a b ");
	EXPECT_EQ(strcmp(result1, "a b "), 0);

	const char* result2 = o1::skipws(" b\n");
	EXPECT_EQ(strcmp(result2, "b\n"), 0);

	const char* result3 = o1::skipws("\t b");
	EXPECT_EQ(strcmp(result3, "b"), 0);

	const char* result4 = o1::skipws("\n\v b");
	EXPECT_EQ(strcmp(result4, "b"), 0);
}

TEST(o1_string_parse, skipws) {
	char* s1 = strdup("a b ");
	char* result1 = o1::skipws(s1);
	EXPECT_EQ(strcmp(result1, "a b "), 0);
	free(s1);
	s1 = nullptr;

	char* s2 = strdup("b\n");
	char* result2 = o1::skipws(s2);
	EXPECT_EQ(strcmp(result2, s2), 0);
	free(s2);
	s2 = nullptr;

	char* s3 = strdup("\t b");
	char* result3 = o1::skipws(s3);
	EXPECT_EQ(strcmp(result3, "b"), 0);
	free(s3);
	s3 = nullptr;

	char* s4 = strdup("\n\v b");
	char* result4 = o1::skipws(s4);
	EXPECT_EQ(strcmp(result4, "b"), 0);
	free(s4);
	s4 = nullptr;
}

TEST(o1_string_parse, memspn) {
	size_t result1 = o1::memspn("go c++!", sizeof("go c++!")-1, "go c+");
	EXPECT_EQ(result1, 6);
}

TEST(o1_string_parse, memcspn) {
	size_t result1 = o1::memcspn("go c++!", sizeof("go c++!")-1, " \t\n");
	EXPECT_EQ(result1, 2);
}

TEST(o1_string_parse, char_class) {
	o1::char_class c1("goc+! {");
	o1::char_class c2("-_a-zA-Z0-9");

	size_t accept1_length = c1.accept("go c++! {}", sizeof("go c++! {}") - 1);
	size_t reject1_length1 = c1.reject("go c++! {}", sizeof("go c++! {}") - 1);
	size_t reject1_length2 = c1.reject("}hi-perl {", sizeof("}hi-perl {") - 1);

	size_t accept2_length = c2.accept("_This-1Is5ATest", sizeof("_This-1Is5ATest") - 1);
	size_t reject2_length = c2.reject(" {}_This-1Is5ATest", sizeof(" {}_This-1Is5ATest") - 1);

	EXPECT_EQ(accept1_length, 9);
	EXPECT_EQ(reject1_length1, 0);
	EXPECT_EQ(reject1_length2, 8);
	EXPECT_EQ(accept2_length, sizeof("_This-1Is5ATest") - 1);
	EXPECT_EQ(reject2_length, 3);
}
