
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)

project(o1CppLib VERSION 0.0.1 LANGUAGES C CXX)

configure_file(src/o1.hh.in o1.hh)

add_library(o1cpp
        src/data/o1.member.hh

        src/data/list/o1.list.d_linked.cc
        src/data/list/o1.list.d_linked.hh
        src/data/list/o1.list.s_linked.cc
        src/data/list/o1.list.s_linked.hh
        src/data/list/o1.list.d_linked_t.hh
        src/data/o1.list.hh

        src/data/queue/o1.queue.cc
        src/data/queue/o1.queue.hh
        src/data/queue/o1.queue_t.hh
        src/data/o1.queue.hh

        src/data/stack/o1.stack.cc
        src/data/stack/o1.stack.hh
        src/data/stack/o1.stack_t.hh
        src/data/o1.stack.hh

        src/o1.data.hh

        src/o1.int.cc
        src/o1.int.hh

        src/o1.debug.cc
        src/o1.debug.hh

        src/string/o1.string.conversion.cc
        src/string/o1.string.conversion.hh
        src/string/o1.string.parse.cc
        src/string/o1.string.parse.hh
        src/string/o1.string.format.cc
        src/string/o1.string.format.hh
        src/o1.string.cc
        src/o1.string.hh

        src/o1.declarative.cc
        src/o1.declarative.hh

        src/o1.logging.cc
        src/o1.logging.hh

        src/errors/o1.invalid-format.cc
        src/errors/o1.invalid-format.hh
        src/o1.compare.hh

        src/o1.time.cc
        src/o1.time.hh

        src/o1.shared_ptr.hh
        src/o1.enum.hh src/o1.promise.hh src/o1.range.hh src/o1.property.hh src/o1.flag_set.hh src/o1.changelog.hh)

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

#        src/data/o1.member.hh
#
#        src/data/list/o1.list.d_linked.hh
#        src/data/list/o1.list.s_linked.hh
#        src/data/list/o1.list.d_linked_t.hh
#        src/data/o1.list.hh
#
#        src/data/queue/o1.queue.hh
#        src/data/queue/o1.queue_t.hh
#        src/data/o1.queue.hh
#
#        src/data/stack/o1.stack.hh
#        src/data/stack/o1.stack_t.hh
#        src/data/o1.stack.hh
#
#        src/o1.data.hh
#
#        src/o1.int.hh
#
#        src/o1.debug.hh
#
#        src/string/o1.string.conversion.hh
#        src/string/o1.string.parse.hh
#        src/string/o1.string.format.hh
#        src/o1.string.hh
#
#        src/o1.declarative.hh
#
#        src/o1.logging.hh
#
#        src/errors/o1.invalid-format.hh
#        src/o1.compare.hh
#
#        src/o1.time.hh
#
#        src/o1.shared_ptr.hh

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
        src/data/list/o1.list.d_linked.test.cc
        src/data/list/o1.list.d_linked_t.test.cc
        src/data/list/o1.list.s_linked.test.cc
        src/data/queue/o1.queue.test.cc
        src/data/queue/o1.queue_t.test.cc
        src/data/stack/o1.stack.test.cc
        src/data/stack/o1.stack_t.test.cc
        src/o1.time.test.cc
        src/string/o1.string.parse.test.cc src/o1.flag_set.test.cc)
target_link_libraries(o1cpp_test gtest_main o1cpp)

include(GoogleTest)
gtest_discover_tests(o1cpp_test)
