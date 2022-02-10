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


#ifndef O1CPPLIB_O1_ENUM_HH
#define O1CPPLIB_O1_ENUM_HH

#include <type_traits>
#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <cstring>

namespace o1 {

	/**
	 * Convert an enum to and from string.
	 * @tparam E the enum
	 */
	template <typename E>
	class EnumConverter { // TODO write UTs
	public:
		static_assert(std::is_enum<E>::value, "EnumConverter can only be used for enum types");
		using conversion_rec = std::tuple<E, const char*, size_t>;
		using conversion_table = std::vector<conversion_rec>;
	protected:
		conversion_table _conversion_table;
		conversion_rec _default;

	public:

		/**
		 *
		 * @param table
		 * @param default_rec
		 */
		explicit EnumConverter(const conversion_table& table, const conversion_rec& default_rec):
			_conversion_table(table),
			_default(default_rec) {
		}

		/**
		 * String to enum conversion.
		 * @param s the string
		 * @return the enum value, based on the conversion table.
		 */
		E aton(const std::string& s) {
			for (const auto& r: _conversion_table) {
				if (std::get<1>(r) == s)
					return std::get<0>(r);
			}
			return std::get<0>(_default);
		}

		/**
		 * String to enum conversion.
		 * @param s the string
		 * @param end optional output parameter, where end of string is stored.
		 * @return the enum value, based on the conversion table.
		 */
		E aton(const char* s, const char** end) {
			for (const auto& r: _conversion_table) {
				if (!strncmp(s, std::get<1>(r), std::get<2>(r))) {
					if (*end)
						*end = s + std::get<2>(r) + 1;
					return std::get<0>(r);
				}
			}
			return std::get<0>(_default);
		}

		/**
		 * Enum to string conversion, based on the conversion table.
		 * @param e enum value to convert.
		 * @return the C string representation, based on the conversion table.
		 */
		const char* ntoa(E e) {
			for (const auto& r: _conversion_table) {
				if (std::get<0>(r) == e)
					return std::get<1>(r);
			}
			return std::get<1>(_default);
		}

	};
}

#endif //O1CPPLIB_O1_ENUM_HH
