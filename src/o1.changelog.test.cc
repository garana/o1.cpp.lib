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
#include "o1.changelog.hh"

namespace {

	struct Datum;
	using node = o1::d_linked::list_t<Datum>::node;

	struct Datum {
		node changelog_node{this};

		static node* getNode(Datum* obj) {
			return &obj->changelog_node;
		}
	};

	TEST(o1_changelog, instantiate) {
		o1::getChangeLog<Datum, Datum::getNode>();
	}

	TEST(o1_changelog, bookKeep) {
		Datum obj;

		auto& changeLog = o1::getChangeLog<Datum, Datum::getNode>();

		EXPECT_EQ(changeLog.modifiedItems().size(), 0);
		o1::getChangeLog<Datum, Datum::getNode>().modified(&obj);
		EXPECT_EQ(changeLog.modifiedItems().size(), 1);
		o1::getChangeLog<Datum, Datum::getNode>().modified(&obj);
		EXPECT_EQ(changeLog.modifiedItems().size(), 1);

		auto* popped = changeLog.modifiedItems().pop_front();
		EXPECT_EQ(popped, &obj);
		EXPECT_EQ(changeLog.modifiedItems().size(), 0);
		EXPECT_EQ(obj.changelog_node.empty(), true);
		EXPECT_EQ(changeLog.modifiedItems().empty(), true);

	}

}
