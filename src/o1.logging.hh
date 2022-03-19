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


#ifndef O1CPPLIB_O1_LOGGING_HH
#define O1CPPLIB_O1_LOGGING_HH

#include <string>
#include <cstddef>
#include <sys/syslog.h>

#ifndef O1_DEFAULT_MAX_DEBUG_SECTIONS
#define O1_DEFAULT_MAX_DEBUG_SECTIONS 4096
#endif

namespace o1 {

	typedef void (*logger_fn)(const char* msg_prefix, const char* msg);
	void set_logger(logger_fn logger);

	/**
	 * assert(3) clone, accepting printf-like format for the message.
	 * @param condition if true, nothing is done; otherwise, the message is
	 *                  reported to stderr, and the process exits.
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void xassert(bool condition, const char* format, ...)
	__attribute__((format(printf, 2, 3)));

	/**
	 * Report the formatted message, and exit. To be used in cases where
	 * an unrecoverable error has found.
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	[[ noreturn ]]
	void fatal(const char* format, ...)
	__attribute__((format(printf, 1, 2)));

	/**
	 * If the LOG_ERR level is enabled, report the formatted message to stderr.
	 * To enable LOG_ERR level, use set_max_log_level(LOG_ERR).
	 * @see set_debug_level(size_t get_debug_section_index, int get_debug_level);
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void error(const char* format, ...)
	__attribute__((format(printf, 1, 2)));

	/**
	 * If the LOG_WARNING level is enabled, report the formatted message to
	 * stderr.
	 * To enable LOG_ERR level, use set_max_log_level(LOG_WARNING).
	 * @see set_debug_level(size_t get_debug_section_index, int get_debug_level);
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void warning(const char* format, ...)
	__attribute__((format(printf, 1, 2)));

	/**
	 * If the LOG_NOTICE level is enabled, report the formatted message to
	 * stderr.
	 * @see set_debug_level(size_t get_debug_section_index, int get_debug_level);
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void notice(const char* format, ...)
	__attribute__((format(printf, 1, 2)));

	/**
	 * If the LOG_INFO level is enabled, report the formatted message to
	 * stderr.
	 * @see set_debug_level(size_t get_debug_section_index, int get_debug_level);
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void info(const char* format, ...)
	__attribute__((format(printf, 1, 2)));

	/**
	 * If both the LOG_DEBUG level, and the debug section are enabled, report
	 * the formatted message to stderr.
	 * @see set_debug_level(size_t get_debug_section_index, int get_debug_level);
	 * @param format printf(3) format of the message.
	 * @param ... printf(3) format arguments.
	 */
	void debug(size_t section, int level, const char* format, ...)
	__attribute__((format(printf, 3, 4)));

	/**
	 * @return iff the @param level is enabled.
	 * @param level Using the syslog(3) levels.
	 * @return
	 */
	bool is_log_level_enabled(int level);

	/**
	 * @return set the min log level to @param level.
	 * @param level possible values are:
	 * - LOG_ERR
	 * - LOG_WARNING
	 * - LOG_NOTICE
	 * - LOG_INFO
	 * - LOG_DEBUG
	 * Enabling a level, also enables the ones above it.
	 * The initial max log level is LOG_NOTICE.
	 */
	void set_max_log_level(int level);

	/**
	 * Get the index of the @param name debug section.
	 * If a debug section does not exist, a new one is created, with log level
	 * 0.
	 */
	size_t get_debug_section_index(const char* name);

	/**
	 * @return the debug level of the @param debug_section.
	 */
	int get_debug_level(size_t debug_section);

	/**
	 * Set the @param debug_section debug level to @param debug_level.
	 * @return the previous debug level of the @param debug_section.
	 */
	int set_debug_level(size_t debug_section, int debug_level);

	/**
	 * Maximum number of debug sections allowed.
	 * The value comes from the environment variable O1_MAX_DEBUG_SECTIONS.
	 * If it's unset, the defined value O1_DEFAULT_MAX_DEBUG_SECTIONS.
	 * @return
	 */
	size_t max_debug_sections();

	std::string demangle(const std::string& s);
};

#endif //O1CPPLIB_O1_LOGGING_HH
