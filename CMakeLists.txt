
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)

project(o1CppLib VERSION 0.0.1 LANGUAGES C CXX)

configure_file(src/o1.hh.in o1.hh)

# TODO organize these
add_library(o1cpp

		src/errors/o1.error.invalid-format.cc
		src/errors/o1.error.invalid-format.hh
		src/errors/o1.error.errno.cc
		src/errors/o1.error.errno.hh

		src/data/o1.event_handlers.hh

		src/data/iterator/o1.iterator_ref.hh
		src/data/iterator/o1.forward_iterator_ref.hh
		src/data/iterator/o1.backward_iterator_ref.hh

		src/data/node/o1.node_t.hh
		src/data/node/o1.d_linked.node.cc
		src/data/node/o1.d_linked.node.hh
		src/data/node/o1.d_linked.node_t.hh
		src/data/node/o1.s_linked.node.cc
		src/data/node/o1.s_linked.node.hh
		src/data/node/o1.s_linked.node_t.hh

		src/data/list/o1.d_linked.list.cc
		src/data/list/o1.d_linked.list.hh
		src/data/list/o1.d_linked.list_t.hh
		src/data/list/o1.s_linked.list.cc
		src/data/list/o1.s_linked.list.hh
		src/data/list/o1.s_linked.list_t.hh

		src/data/queue/o1.d_linked.queue.hh
		src/data/queue/o1.d_linked.queue_t.hh
		src/data/queue/o1.s_linked.queue.hh
		src/data/queue/o1.s_linked.queue_t.hh

		src/data/stack/o1.d_linked.stack.hh
		src/data/stack/o1.d_linked.stack_t.hh
		src/data/stack/o1.s_linked.stack.hh
		src/data/stack/o1.s_linked.stack_t.hh

		src/data/hash/o1.hash.table_t.hh
		src/data/hash/o1.hash.buckets_t.hh
		src/data/hash/o1.hash.bucket_t.hh
		src/data/hash/o1.hash.ops_t.hh
		src/data/hash/o1.hash.ops_t.cc

		src/data/o1.list.hh
		src/data/o1.queue.hh
		src/data/o1.stack.hh

		src/o1.changelog.hh
		src/o1.compare.hh
		src/o1.data.hh
		src/o1.debug.cc
		src/o1.debug.hh
		src/o1.declarative.cc
		src/o1.declarative.hh
		src/o1.enum.hh
		src/o1.flag_set.hh
		src/o1.int.cc
		src/o1.int.hh
		src/o1.logging.cc
		src/o1.logging.hh
		src/o1.promise.hh
		src/o1.property.hh
		src/o1.range.hh
		src/o1.string.cc
		src/o1.string.hh
		src/o1.time.cc
		src/o1.time.hh
		src/string/o1.string.conversion.cc
		src/string/o1.string.conversion.hh
		src/string/o1.string.format.cc
		src/string/o1.string.format.hh
		src/string/o1.string.parse.cc
		src/string/o1.string.parse.hh
		src/o1.math.hh
		src/data/hash/o1.hash.sizing_strategy.cc
		src/data/hash/o1.hash.sizing_strategy.hh
		src/data/hash/o1.hash.conf.hh

		src/memory/allocator/o1.memory.stdlib_allocator.cc
		src/memory/allocator/o1.memory.stdlib_allocator.hh
		src/memory/allocator/o1.memory.mmap_allocator.cc
		src/memory/allocator/o1.memory.mmap_allocator.hh

		src/memory/pool/o1.memory.pooled.hh
		src/memory/pool/o1.memory.pool.hh
		src/memory/pool/o1.memory.pool.chunk.hh
		src/memory/pool/o1.memory.pool_strategy.cc
		src/memory/pool/o1.memory.pool_strategy.hh
		src/memory/pool/o1.memory.pool.free_cache.hh
		src/memory/pool/o1.memory.pool.chunked.hh
		src/memory/pool/o1.memory.pool_base.cc
		src/memory/pool/o1.memory.pool_base.hh
		src/memory/pool/o1.memory.pool.metrics.cc
		src/memory/pool/o1.memory.pool.metrics.hh

		src/data/hash/o1.hash.node_t.hh
)

install(TARGETS o1cpp DESTINATION lib/${PROJECT_NAME})

set_target_properties(o1cpp PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(o1cpp PROPERTIES SOVERSION ${PROJECT_VERSION})

# TODO exclude *.test.cc
file(GLOB SOURCES src/*.cc)
file(GLOB HEADERS
		src/*.hh
		src/*/*.hh
		src/*/*/*.hh
		)
#install(FILES ${HEADERS} DESTINATION include/o1.cpp.lib)
install(DIRECTORY src/ DESTINATION include/o1.cpp.lib)

target_include_directories(o1cpp PUBLIC "${PROJECT_BINARY_DIR}")

## Unit Testing Setup

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(o1cpp_test
		src/data/hash/o1.hash.ops_t.test.cc
		src/data/hash/o1.hash.sizing_strategy.test.cc
		src/data/hash/o1.hash.table_t.test.cc

		src/data/list/o1.d_linked.list.test.cc
		src/data/list/o1.d_linked.list_t.test.cc
		src/data/list/o1.s_linked.list.test.cc

		src/data/queue/o1.s_linked.queue.test.cc
		src/data/queue/o1.s_linked.queue_t.test.cc

		src/data/stack/o1.s_linked.stack_t.test.cc
		src/data/stack/o1.stack.test.cc

		src/memory/pool/o1.memory.pool.test.cc

		src/string/o1.string.parse.test.cc

		src/o1.changelog.test.cc
		src/o1.flag_set.test.cc
		src/o1.logging.test.cc
		src/o1.math.test.cc
		src/o1.range.test.cc
		src/o1.sizes.test.cc
		src/o1.time.test.cc
)

target_link_libraries(o1cpp_test gtest_main o1cpp)

include(GoogleTest)
gtest_discover_tests(o1cpp_test)
