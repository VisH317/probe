cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(torch-uuid)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

file(GLOB SOURCES "src/*.cpp")

add_executable(torch-uuid ${SOURCES})
set_target_properties(torch-uuid PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(torch-uuid PRIVATE include)  
target_link_libraries(torch-uuid "${TORCH_LIBRARIES}")
set_property(TARGET torch-uuid PROPERTY CXX_STANDARD 17)

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