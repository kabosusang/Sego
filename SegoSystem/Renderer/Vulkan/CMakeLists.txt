cmake_minimum_required(VERSION 3.20)

project(
    Vulkanwrapper
    LANGUAGES C CXX
    VERSION 0.1
    DESCRIPTION "Vulkan C++ wrapper"
)

aux_source_directory(./src engine_src)
message("source files: ${engine_src}")

add_library(vulkanwrapper STATIC ${engine_src})
target_include_directories(vulkanwrapper PUBLIC include)
target_compile_features(vulkanwrapper PRIVATE cxx_std_17)

