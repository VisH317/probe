cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(torch-uuid)
include(CTest)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/jitter/*.cpp")

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

target_link_libraries(torch-probe "${TORCH_LIBRARIES}")
target_include_directories(torch-probe PRIVATE include)
target_include_directories(torch-probe PRIVATE include/jitter)


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