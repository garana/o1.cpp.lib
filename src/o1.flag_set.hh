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


#ifndef O1CPPLIB_O1_FLAG_SET_HH
#define O1CPPLIB_O1_FLAG_SET_HH

#include <type_traits>
#include <cstdint>
#include <utility>
#include <vector>

namespace o1 {

	/**
	 * flag set, up to 128 flag values
	 * @tparam T enum type
	 * @tparam S unsigned integral type, to store the flags.
	 */
	template <typename T, typename S = uint32_t>
	class flag_set {

		static_assert(
			std::is_enum<T>::value,
			"flag_set<T,S> instantiated with non-enum type for T"
		);

		static_assert(
			std::is_integral<S>::value,
			"flag_set<T,S> instantiated with non-integral type for S"
		);

		static_assert(
			std::is_unsigned<S>::value,
			"flag_set<T,S> instantiated with non-unsigned type for S"
		);

		// TODO add a check if _flags can hold all T values.
		S _flags{0};

	public:

		using storage_t = S;

		flag_set() = default;
		flag_set(const flag_set& that): _flags(that._flags) { }
		explicit flag_set(S flags): _flags(flags) { }
		flag_set(flag_set&& that) noexcept : _flags(that._flags) { that._flags = 0; }

		static S mask(T flag) {
			// TODO check the flag value fit in S.
			return static_cast<S>(1) << static_cast<unsigned>(flag);
		}

		flag_set& set(T flag) {
			_flags |= mask(flag);
			return *this;
		}

		flag_set& clear(T flag) {
			_flags &= ~mask(flag);
			return *this;
		}

		flag_set& assign(T flag, bool status) {
			return status ? set(flag) : clear(flag);
		}

		bool isSet(T flag) const {
			return (_flags & mask(flag)) != 0;
		}

		flag_set& operator = (const flag_set& that) {
			_flags = that._flags;
			return *this;
		}

		flag_set& operator = (storage_t new_flags) {
			_flags = new_flags;
			return *this;
		}

	};

	/**
	 * if we need to store more than 128 flag values, use a bit vector.
	 * @tparam T enum type
	 */
	template <typename T>
	class flag_set<T, std::vector<bool>> {

		static_assert(
			std::is_enum<T>::value,
			"flag_set<T,S> instantiated with non-enum type for T"
		);

		std::vector<bool> _flags;

		static size_t index(T flag) {
			return static_cast<size_t>(flag);
		}

		void _maybe_resize(T flag) {
			auto _index = index(flag);
			if (_flags.size() <= _index)
				_flags.resize(_index+1, false);
		}

	public:
		flag_set() = default;

		explicit flag_set(std::vector<bool> flags):
			_flags(std::move(flags)) { }

		flag_set(const flag_set& that): _flags(that._flags) { }

		flag_set(flag_set&& that) noexcept :
			_flags(std::move(that.flags)) { }

		flag_set& set(T flag) {
			_maybe_resize(flag);
			_flags[index(flag)] = true;
			return *this;
		}

		flag_set& clear(T flag) {
			_maybe_resize(flag);
			_flags[index(flag)] = false;
			return *this;
		}

		bool isSet(T flag) {
			return _flags[index(flag)];
		}

	};

}

#endif //O1CPPLIB_O1_FLAG_SET_HH
