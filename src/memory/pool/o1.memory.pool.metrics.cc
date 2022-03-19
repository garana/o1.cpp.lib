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

#include "../../o1.logging.hh"
#include "o1.memory.pool.metrics.hh"
using o1::memory::alloc_metrics;

void alloc_metrics::allocated(
	bool new_chunk,
	bool chunk_de_idled,
	const alloc_metrics::pool_info* poolInfo
) {
	check_consistency();

	if (new_chunk) {
		items.total += poolInfo->items_per_chunk;
		items.idle += poolInfo->items_per_chunk;
		bytes.total += poolInfo->bytes_per_chunk;
		bytes.idle += poolInfo->bytes_per_chunk;
		chunks.total++;
	}

	if (chunk_de_idled) {
		chunks.idle--;
	}

	items.idle--;
	bytes.idle -= poolInfo->item_size;

	check_consistency();
}

void alloc_metrics::deallocated(
	bool chunk_freed,
	bool chunk_idled,
	const alloc_metrics::pool_info* poolInfo
) {
	check_consistency();

	items.idle++;
	bytes.idle += poolInfo->item_size;

	if (chunk_idled)
		chunks.idle++;

	if (chunk_freed) {
		items.total -= poolInfo->items_per_chunk;
		items.idle -= poolInfo->items_per_chunk;
		bytes.total -= poolInfo->bytes_per_chunk;
		bytes.idle -= poolInfo->bytes_per_chunk;
		chunks.total--;
		chunks.idle--;
	}

	check_consistency();
}

void o1::memory::alloc_metrics::check_consistency() {
	o1::xassert(items.idle <= items.total, "internal bug");
	o1::xassert(bytes.idle <= bytes.total, "internal bug");
	o1::xassert(chunks.idle <= chunks.total, "internal bug");
}
