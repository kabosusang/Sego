cmake_minimum_required(VERSION 3.0.0)

project(Sego)
set(SRC main.cpp)

#增加-std=c++17
set(CMAKE_CXX_STANDARD 17)

# 输出目录
set(output_bin ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${output_bin})

#头文件
#SegoHead
include_directories(${PROJECT_SOURCE_DIR}/SegoSystem)
include_directories(${PROJECT_SOURCE_DIR}/Resource/Editor)
include_directories(${PROJECT_SOURCE_DIR}/Resource)
#
include_directories(${PROJECT_SOURCE_DIR}/vendor/)
include_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/glm)
include_directories(${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/tinyobjloader)
include_directories(${PROJECT_SOURCE_DIR}/vendor/rapidxml)
include_directories(${PROJECT_SOURCE_DIR}/vendor/stb)
include_directories(${PROJECT_SOURCE_DIR}/SegoUI/imgui)


#add subdirectory
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)


#Vulkan
include_directories(D:/Vulkan1.3.246.1/Include)

#lib  
# 包含库路径
link_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/build/src) # 静态

#Vulkan lib
link_directories(D:/Vulkan1.3.246.1/Lib)


# glfw
link_libraries(vulkan-1)
link_libraries(glfw3)

#源文件src
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Resource/Editor SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SegoSystem/Core CORE_LIST)
# 添加SegoUI文件夹下的源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SegoUI/imgui SEGOUI_SRC_LIST)
# 添加SegoSystem文件夹下的源文件

file(GLOB SOURCES "SegoSystem/Log/*.cpp")
file(GLOB THREAD "SegoSystem/ThreadPool/*.cpp")
file(GLOB WINDOW "SegoSystem/Window/*.cpp")
file(GLOB CAMERA "SegoSystem/camera/*.cpp")


#添加vulkan渲染源文件
file(GLOB APPLICATION "SegoSystem/Core/Application/*.cpp")
file(GLOB VULKAN "SegoSystem/Renderer/Vulkan/*.cpp")
file(GLOB SGUI "SegoSystem/Renderer/SgUI/*.cpp")

#添加渲染数据文件
file(GLOB MESH "SegoSystem/Core/Mesh/*.cpp")
file(GLOB MATERIAL "Resource/Material/*.cpp")



#APPEND LIST
list(APPEND SRC_LIST ${CORE_LIST})
list(APPEND SRC_LIST ${SEGOUI_SRC_LIST})
list(APPEND SRC_LIST ${SOURCES})
list(APPEND SRC_LIST ${THREAD})
list(APPEND SRC_LIST ${WINDOW})
list(APPEND SRC_LIST ${CAMERA})
list(APPEND SRC_LIST ${APPLICATION})
list(APPEND SRC_LIST ${VULKAN})
list(APPEND SRC_LIST ${SGUI})

#Data 
list(APPEND SRC_LIST ${MESH})
list(APPEND SRC_LIST ${MATERIAL})


#add exe
add_executable(editor  ${SRC_LIST})

#add link
target_link_libraries(editor PRIVATE imgui glfw3)



