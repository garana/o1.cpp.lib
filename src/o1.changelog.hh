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


#ifndef O1CPPLIB_O1_CHANGELOG_HH
#define O1CPPLIB_O1_CHANGELOG_HH

#include <cstddef>
#include "data/list/o1.list.d_linked_t.hh"

namespace o1 {

	/**
	 * Keep modified objects in a list, being able to sweep through
	 * them later.
	 * @tparam T type of objects to track.
	 * @tparam node_offset d_linked_t::node offset in T
	 */
	template <typename T, std::ptrdiff_t node_offset>
	class changelog {

	public:
		using list_t = o1::list::d_linked_t<T, node_offset>;
		using node_t = typename o1::list::d_linked_t<T, node_offset>::node;

		list_t& modifiedItems() {
			static list_t _modifiedItems;
			return _modifiedItems;
		}

		void modified(T* obj) {
			auto _node =
				o1::member<T, node_t, node_offset>::node2member(obj);

			// if it's already modified, do nothing.
			if (!_node->isEmpty())
				return;

			modifiedItems().push_back(obj);
		}

	};

	template <typename T, std::ptrdiff_t node_offset>
	changelog<T, node_offset>& getChangeLog() {
		static changelog<T, node_offset> _changelog;
		return _changelog;
	}

}

#endif //O1CPPLIB_O1_CHANGELOG_HH
