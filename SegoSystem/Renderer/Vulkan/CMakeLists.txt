cmake_minimum_required(VERSION 3.20)

project(
    Vulkanwrapper
    LANGUAGES C CXX
    VERSION 0.1
    DESCRIPTION "Vulkan C++ wrapper"
)

add_library(vulkanwrapper STATIC src/Vk_Console.cpp src/Vk_Console.cpp src/Vk_Image.cpp src/Vk_Memory.cpp)
target_include_directories(vulkanwrapper PUBLIC include)
target_compile_features(vulkanwrapper PRIVATE cxx_std_17)

