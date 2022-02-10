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


#ifndef O1CPPLIB_O1_SHARED_PTR_HH
#define O1CPPLIB_O1_SHARED_PTR_HH

#include <cstdint>
#include <cstdlib>
#include <utility>
#include <memory>


// TODO could place the objects to remove in a list?
/**
 * Light std::shared_ptr alike implementation:
 * - Not thread-safe, no atomic overhead, etc.
 * - No array support
 * - No operators overloaded (yet).
 * - No support for weak_ptr (always owning).
 * - The control block is directly inside the referenced object
 * - Easy conversion from/to shared_ptr and plain old pointer.
 * - Can be allocated with new
 *
 */
namespace o1 {

	template <class Rec, class Deleter> class shared_ptr;

	class shared_pointable {

	private:
		uint32_t _ref_cnt = 0;

	public:
		shared_pointable() = default;
		shared_pointable(const shared_pointable& that): _ref_cnt(0) { }
		shared_pointable(shared_pointable&& that) noexcept : _ref_cnt(that._ref_cnt) { that._ref_cnt = 0; }
		~shared_pointable() {
			// TODO add a check on ~shared_pointable if _ref_cnt != 0
		}

		template <class Rec, class Deleter>
		friend class shared_ptr;
	};

	template <class Rec>
	struct get_shared_pointable {
		static inline shared_pointable* get(Rec* r) {
			return dynamic_cast<shared_pointable*>(r);
		}
	};

	template <class Rec, class Deleter = std::default_delete<Rec> >
	class shared_ptr { // TODO garana@ write UTs
	protected:
		Rec* _p = nullptr;

		inline void _ref() {
			if (_p)
				get_shared_pointable<Rec>::get(_p)->_ref_cnt++;
			// dynamic_cast<shared_pointable*>(_p)->_ref_cnt++;
		}

		inline void _deref() {
			if (_p) {
				auto& cnt = get_shared_pointable<Rec>::get(_p)->_ref_cnt;
				if (!--cnt)
					(Deleter(_p));
				_p = nullptr;
			}
		}

	public:
		shared_ptr() = default;

		explicit shared_ptr(Rec* p): _p(p) {
			_ref();
		}

		shared_ptr(const shared_ptr& that): _p(that._p) {
			_ref();
		}

		shared_ptr(shared_ptr&& that) noexcept : _p(std::move(that._p)) {
			that._p = nullptr;
		}

		~shared_ptr() {
			_deref();
		}

		shared_ptr& operator = (Rec* p) {
			if (_p != p) {
				_deref();
				_p = p;
				_ref();
			}
			return *this;
		}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-unhandled-self-assignment"
		shared_ptr& operator = (const shared_ptr& that) {
			if (_p != that._p) {
				_deref();
				_p = that._p;
				_ref();
			}
			return *this;
		}
#pragma clang diagnostic pop

		shared_ptr& operator = (shared_ptr&& that)  noexcept {
			if (_p != that._p) {
				_deref();
				_p = that._p;
				_ref();
				that._deref();
				that._p = nullptr;
			}
			return *this;
		}

		Rec* operator()() { return _p; }

		const Rec* get() const { return _p; }
		      Rec* get()       { return _p; }


		Rec* operator->() {
			return _p;
		}

		const Rec* operator->() const {
			return _p;
		}

#if 0
		shared_ptr& operator++() { // prefix
			_deref();
			_p++;
			_ref();
			return *this;
		}

		shared_ptr operator++(int _unused) { // postfix
			shared_ptr tmp(*this);
			_deref();
			_p++;
			_ref();
			return tmp;
		}

		shared_ptr& operator += (int delta) {
			_deref();
			_p += delta;
			_ref();
			return *this;
		}

		shared_ptr& operator -= (int delta) {
			_deref();
			_p -= delta;
			_ref();
			return *this;
		}
#endif

		bool operator < (const shared_ptr& that) {
			return *this < that._p;
		}

		bool operator < (Rec* p) {
			return _p < p;
		}

		bool operator <= (const shared_ptr& that) {
			return *this <= that._p;
		}

		bool operator <= (Rec* p) {
			return _p <= p;
		}

		bool operator > (const shared_ptr& that) {
			return *this > that._p;
		}

		bool operator > (Rec* p) {
			return _p > p;
		}

		bool operator >= (const shared_ptr& that) {
			return *this >= that._p;
		}

		bool operator >= (Rec* p) {
			return _p >= p;
		}

		bool operator != (const shared_ptr& that) {
			return *this != that._p;
		}

		bool operator != (Rec* p) {
			return _p != p;
		}

		bool operator == (const shared_ptr& that) {
			return *this == that._p;
		}

		bool operator == (Rec* p) {
			return _p == p;
		}

	};

};

#endif
