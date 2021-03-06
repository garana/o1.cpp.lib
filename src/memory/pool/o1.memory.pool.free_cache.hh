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


#ifndef O1CPPLIB_O1_MEMORY_POOL_FREE_CACHE_HH
#define O1CPPLIB_O1_MEMORY_POOL_FREE_CACHE_HH

#include "../../data/list/o1.s_linked.list.hh"
#include "o1.memory.pool.hh"

namespace o1 {

	namespace memory {

		template <typename T>
		class pool<T, PoolStrategy::freeCache>: public pool_base {
		public:
			using list_t = o1::s_linked::list;
			using node_t = typename list_t::node;

			static_assert(sizeof(T) >= sizeof(node_t), "Refusing to pool small objects");

		private:

			static list_t& freeObjects() {
				static list_t _freeObjects;
				return _freeObjects;
			}

			static const alloc_metrics::pool_info* allocMetricsPoolInfo() {
				static alloc_metrics::pool_info metrics_pool_info{
					.items_per_chunk = 1,
					.bytes_per_chunk = sizeof(T),
					.item_size = sizeof(T),
				};
				return &metrics_pool_info;
			}

		public:
			pool():
				pool_base(o1::demangle(typeid(T).name()) + (":" + ntoa(PoolStrategy::freeCache))) {
			}

			void* alloc() {
				auto node = freeObjects().pop_front();

				if (node == nullptr) {
					_metrics.allocated(true, false, allocMetricsPoolInfo());
					return std::malloc(sizeof(T));
				}

				_metrics.allocated(false, false, allocMetricsPoolInfo());
				return static_cast<void*>(node);
			}

			void dealloc(void* p) {
				_metrics.deallocated(false, false, allocMetricsPoolInfo());
				freeObjects().push_front(static_cast<node_t*>(p));
				// TODO keep a max number of free objects?
			}

		};

	}

}

#endif //O1CPPLIB_O1_MEMORY_POOL_FREE_CACHE_HH
