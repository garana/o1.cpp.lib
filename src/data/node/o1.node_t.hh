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


#ifndef O1CPPLIB_O1_NODE_T_HH
#define O1CPPLIB_O1_NODE_T_HH

namespace o1 {

	/**
	 * Typed node, holding a pointer to the datum (T).
	 * @tparam T
	 */
	template <typename T>
	class node_t {
		T* _ref{nullptr};
	public:
		node_t() = delete;

		explicit node_t(T* ref): _ref(ref) { }

		node_t(node_t&& that) noexcept {
			_ref = that._ref;
			that._ref = nullptr;
		}

		[[nodiscard]] inline T* ref() { return _ref; }
		[[nodiscard]] inline const T* ref() const { return _ref; }

		[[nodiscard]] inline static T* ref(node_t<T>* node) {
			return node == nullptr ? nullptr : node->ref();
		}

		[[nodiscard]] inline static const T* ref(const node_t<T>* node) {
			return node == nullptr ? nullptr : node->ref();
		}

	};


}

#endif //O1CPPLIB_O1_NODE_T_HH
