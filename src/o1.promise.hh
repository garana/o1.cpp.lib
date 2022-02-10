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


#ifndef O1CPPLIB_O1_PROMISE_HH
#define O1CPPLIB_O1_PROMISE_HH


#include <memory>
#include <functional>
#include <utility>

namespace o1 {

	template <typename C>
	class promise {
	public:
		typedef C result_t;
		typedef promise<C> promise_t;
		typedef std::shared_ptr< promise_t > shared_ptr;
		typedef std::function<void(C)> ok_f;
		typedef std::function<void(const std::exception*)> err_f;

		static shared_ptr make_shared() { return std::make_shared< promise_t >(); }

	private:
		C _result;
		std::shared_ptr< std::exception > _exception;

		enum class status_t {
			stNONE = 0,
			stResolved = 1,
			stRejected = 2
		};

		status_t _status{status_t::stNONE};

		struct cbn {
			cbn(ok_f okFn, err_f errFn):
				ok(std::move(okFn)),
				err(std::move(errFn)),
				promise(std::make_shared<promise_t>()) { }
			ok_f ok;
			err_f err;
			cbn* next = NULL;
			shared_ptr promise = NULL;
		};

		cbn* _cbl = NULL;
		cbn** _tail = NULL;

		void _run_ok() {
			for (cbn* node = _cbl; node != NULL; node = node->next) {
				if (node->ok)
					node->ok(_result);

				promise_t* p = node->promise.get();

				if (p != NULL)
					p->resolve(_result);
			}
		}

		void _run_err() {
			for (cbn* node = _cbl; node != NULL; node = node->next) {
				if (node->err)
					node->err(_exception.get());

				promise_t* p = node->promise.get();

				if (p != NULL)
					p->reject(_exception);
			}
		}

	public:

		promise(): _cbl(NULL), _tail(&_cbl) { }

		explicit promise(const C& initialValue): _result(initialValue), _cbl(NULL), _tail(&_cbl) { }

		promise(const promise& that) = delete;

		promise(promise&& that) noexcept :
			_result(that._result),
			_exception(std::move(that._exception)),
			_status(that._status),
			_tail(&_cbl) { }

		~promise() {
			while (NULL != _cbl) {
				_cbl = _cbl->next; // TODO memory leak?
			}
		}

		static promise resolved(const C& value) {
			promise ret(value);
			ret._result = value;
			ret._status = status_t::stResolved;
			return ret;
		}

		static promise rejected(std::shared_ptr< std::exception >& value) {
			promise ret;
			ret._exception = value;
			ret._status = status_t::stRejected;
			return ret;
		}

		bool is_ready() const { return _status != status_t::stNONE; }
		bool is_resolved() const { return _status == status_t::stResolved; }
		bool is_rejected() const { return _status == status_t::stRejected; }

		cbn* new_node(ok_f okFn, err_f errFnm) {
			cbn* node = new cbn(okFn, errFnm);
			*_tail = node; _tail = &node->next;

			switch (_status) {
				case status_t::stResolved:
					_run_ok();
					break;

				case status_t::stRejected:
					_run_err();
					break;
			}

			return node;
		}


		// cbn* new_node(std::shared_ptr< promise_t> _promise) {
		// 	cbn* node = new cbn(_promise);
		// 	*_tail = node; _tail = &node->next;
		// 	if (_ready) {
		// 		if (_resolved)
		// 			_run_ok();
		// 		else
		// 			_run_err();
		// 	}
		// 	return node;
		// }


		std::shared_ptr< promise_t >
		then(std::function<void(const C&)> okFn) {
			return new_node(okFn, NULL)->promise;
		}

		std::shared_ptr< promise_t >
		then(std::function<void(const C&)> okFn, std::function<void(const C&)> errFn) {
			return new_node(okFn, errFn)->promise;
		}

		std::shared_ptr< promise_t >
		error(std::function<void(const C&)> errFn) {
			return new_node(NULL, errFn)->promise;
		}

		std::shared_ptr< promise_t >
		tie(std::shared_ptr< promise_t >& p) {

			new_node(NULL, NULL)->promise = p;

			if (is_resolved())
				p->resolve(_result);
			else if (is_rejected())
				p->reject(_exception);

			return p;

		}


		void resolve(const C& result) {
			_status = status_t::stResolved;
			_result = result;
			_run_ok();
		}

		void reject(std::shared_ptr< std::exception >& _error) {
			_status = status_t::stRejected;
			_exception = _error;
			_run_err();
		}

	};

};

#endif //O1CPPLIB_O1_PROMISE_HH
