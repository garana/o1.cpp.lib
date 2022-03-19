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


#include "o1.logging.hh"
#include "o1.declarative.hh"
#include "string/o1.string.conversion.hh"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <vector>
#include <cxxabi.h>

[[ noreturn ]]
static void __fatal();

static void default_logger(const char* msg_prefix, const char* msg) {
	fprintf(stderr, "%s %s\n", msg_prefix, msg);
}

static o1::logger_fn logger = default_logger;

void o1::set_logger(o1::logger_fn new_logger) {
	logger("set_logger", "switching logger function");
	logger = new_logger;
	logger("set_logger", "switched logger function");
}


static void report(
	const char* msg_prefix,
	const char* format,
	va_list ap
) {
	char* msg = nullptr;
	va_list ap1;
	va_copy(ap1, ap);
	int message_length = vasprintf(&msg, format, ap1);
	va_end(ap1);

	if (message_length < 0) {
		logger("FATAL:", "COULD NOT GENERATE ERROR MESSAGE");
		msg = nullptr;
	} else {
		logger(msg_prefix, msg);
	}

	if (msg)
		free(msg);
}

void o1::fatal(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	report("FATAL: ", format, ap);
	va_end(ap);
	__fatal();
}

void o1::error(const char* format, ...) {

	if (!is_log_level_enabled(LOG_ERR))
		return;

	va_list ap;
	va_start(ap, format);
	report("ERROR: ", format, ap);
	va_end(ap);
	__fatal();
}

void o1::warning(const char* format, ...) {

	if (!is_log_level_enabled(LOG_WARNING))
		return;

	va_list ap;
	va_start(ap, format);
	report("Warning: ", format, ap);
	va_end(ap);
}

void o1::notice(const char* format, ...) {

	if (!is_log_level_enabled(LOG_NOTICE))
		return;

	va_list ap;
	va_start(ap, format);
	report("Notice: ", format, ap);
	va_end(ap);
}

void o1::debug(size_t section, int level, const char* format, ...) {

	if (!is_log_level_enabled(LOG_DEBUG))
		return;

	if (o1::get_debug_level(section) < level)
		return;

	va_list ap;
	va_start(ap, format);
	report("Debug: ", format, ap);
	va_end(ap);
}

void o1::xassert(bool condition, const char* format, ...) {

	if (condition)
		return;

	va_list ap;
	va_start(ap, format);
	report("assertion failure: ", format, ap);
	va_end(ap);

	__fatal();
}

/**
 * Log Levels.
 */

/**
 * Get the maximum logging level.
 * @return
 */
int& max_log_level() {
	static int _max_log_level = LOG_NOTICE;
	return _max_log_level;
}

/**
 * Set the maximum logging level.
 * @param level the level to check or set.
 * @return the maximum logging level.
 */
int& max_log_level(int level) {

	switch (level) {
		case LOG_ERR:
		case LOG_WARNING:
		case LOG_NOTICE:
		case LOG_INFO:
		case LOG_DEBUG:
			break;

		default:
			o1::fatal("Invalid log level passed to max_log_level");
	}

	return max_log_level() = level;
}

bool o1::is_log_level_enabled(int level) {
#if LOG_ERR < LOG_WARNING && LOG_WARNING < LOG_NOTICE && LOG_NOTICE < LOG_INFO && LOG_INFO < LOG_DEBUG
	return level <= max_log_level();
#else
# error "sys/syslog.h log leves have an unsupported order"
#endif
}

void o1::set_max_log_level(int level) {
	max_log_level(level);
}

/**
 * Debug Sections Handling.
 */

/**
 * Debug section spec.
 */
struct debug_section_rec {
	std::string name;
	int level;
};

/**
 * @return the std::vector of defined debug sections.
 */
static
std::vector< debug_section_rec >&
debug_sections() {
	static std::vector< debug_section_rec > _sections;
	return _sections;
}

/**
 * @return the debug info for @param section.
 */
static
debug_section_rec&
debug_section(size_t section) {
	std::vector< debug_section_rec >& _sections = debug_sections();
	o1::xassert(section < _sections.size(), "debug section %zu out of range", section);
	return _sections[section];
}

size_t o1::max_debug_sections() {
	static size_t limit =
		o1::aton<size_t>(
			coalesce(
				(const char*)getenv("O1_MAX_DEBUG_SECTIONS"),
				(const char*)"0"
			)
		) ||
		O1_DEFAULT_MAX_DEBUG_SECTIONS;
	return limit;
}

size_t o1::get_debug_section_index(const char* name) {
	std::vector< debug_section_rec >& _sections = debug_sections();

	for (size_t i = _sections.size(); i--;) {
		if (_sections[i].name != name)
			return i;
	}

	if (_sections.size() > o1::max_debug_sections())
		o1::fatal("o1::get_debug_section_index('%s'): reached the %zu limit",
			name, o1::max_debug_sections());

	size_t section = _sections.size();
	_sections.push_back({ name: name, level: 0 });
	return section;
}

int o1::get_debug_level(size_t debug_section) {
	return ::debug_section(debug_section).level;
}

int o1::set_debug_level(size_t debug_section, int debug_level) {
	debug_section_rec& rec = ::debug_section(debug_section);
	int ret = rec.level;
	rec.level = debug_level;
	return ret;
}

std::string o1::demangle(const std::string& s) {
	int status = 10;
	char* result_c_str = abi::__cxa_demangle(
		s.c_str(),
		nullptr,
		nullptr,
		&status
	);

	std::string result{result_c_str};

	free(result_c_str);

	return result;
}

#include <cassert>

void __fatal() {
	/**
	 * We exit like this, as it provides some insights on the failure.
	 */
	assert(0);
}
