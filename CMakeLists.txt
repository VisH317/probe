cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(torch-uuid)
include(CTest)
include(FetchContent)

find_package(Torch REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)


file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/jitter/*.cpp" "src/jitter/main/*.cpp" "src/jitter/worker/*.cpp")

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -pthread -fsanitize=address")
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# old test executable (not needed anymore)
# add_executable(torch-uuid ${SOURCES})
# set_target_properties(torch-uuid PROPERTIES LINKER_LANGUAGE CXX)

# target_include_directories(torch-uuid PRIVATE include)  
# target_link_libraries(torch-uuid "${TORCH_LIBRARIES}")
# set_property(TARGET torch-uuid PROPERTY CXX_STANDARD 17)

#library setup

add_library(torch-probe SHARED ${SOURCES})
set_target_properties(torch-probe PROPERTIES SOVERSION 1)
set_target_properties(torch-probe PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET torch-probe PROPERTY CXX_STANDARD 17)

target_link_libraries(torch-probe Boost::program_options)
target_link_libraries(torch-probe "${TORCH_LIBRARIES}")
target_include_directories(torch-probe PRIVATE include)
target_include_directories(torch-probe PRIVATE include/jitter)
target_include_directories(torch-probe PRIVATE include/jitter/main)
target_include_directories(torch-probe PRIVATE include/jitter/messages)
target_include_directories(torch-probe PRIVATE include/jitter/worker)

enable_testing()

# file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "test/*.cpp" "test/*.hpp")

# original test
add_executable(probe-test "test/main.cpp")
set_target_properties(probe-test PROPERTIES SOVERSION 1)
set_target_properties(probe-test PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET probe-test PROPERTY CXX_STANDARD 17)

target_link_libraries(probe-test torch-probe)
target_link_libraries(probe-test Boost::program_options)
target_link_libraries(probe-test "${TORCH_LIBRARIES}")
target_link_libraries(probe-test GTest::gtest_main)
target_include_directories(probe-test PRIVATE include)
target_include_directories(probe-test PRIVATE include/jitter)
target_include_directories(probe-test PRIVATE include/jitter/main)
target_include_directories(probe-test PRIVATE include/jitter/messages)
target_include_directories(probe-test PRIVATE include/jitter/worker)

include(GoogleTest)
gtest_discover_tests(probe-test)


# setup installation
include(GNUInstallDirs)
install(TARGETS torch-probe LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#tests

# add_executable(uuid-test "test/testMain.cpp")
# add_test(NAME maintest COMMAND uuid-test)

# add_executable(Test)


# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET torch-uuid
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:example-app>)
endif (MSVC)