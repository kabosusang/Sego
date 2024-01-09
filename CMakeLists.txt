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
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/SegoSystem)
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

set(GLFW_BUILD_ONCE OFF CACHE BOOL "Build 3dr only once")

add_subdirectory(vendor/imgui)
# 添加 subdirectory 的条件判断
if(NOT GLFW_BUILD_ONCE)
    add_subdirectory(vendor/glfw)
    add_subdirectory(vendor/freetype-2.13.2)
  set(GLFW_BUILD_ONCE ON CACHE BOOL "Build 3dr only once" FORCE)
endif()

#Vulkan
set(VULKAN_PATH $ENV{VULKAN_SDK})

message("VULKAN_SDK: ${VULKAN_PATH}")
include_directories(${VULKAN_PATH}/Include)
link_directories(${VULKAN_PATH}/Lib)

#lib  
# 包含库路径
link_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/build/src) # 静态

# glfw
link_libraries(vulkan-1)
link_libraries(glfw3)
#rttr
include_directories(${PROJECT_SOURCE_DIR}/vendor/rttr/src)
link_directories(${PROJECT_SOURCE_DIR}/vendor/rttr/build/src/rttr)

#freetype
include_directories(${PROJECT_SOURCE_DIR}/vendor/freetype-2.13.2/include)
link_directories(${PROJECT_SOURCE_DIR}/vendor/freetype-2.13.2/build)

#lua5.3
include_directories(${PROJECT_SOURCE_DIR}/vendor/lua/src)
link_directories(${PROJECT_SOURCE_DIR}/vendor/lua/build)


#Assimp
include_directories(${PROJECT_SOURCE_DIR}/vendor/vendor/Assimp)
link_directories(${PROJECT_SOURCE_DIR}/vendor/Assimp/build/lib)

#源文件src
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Resource/Editor SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SegoSystem/Core DATA_LIST)
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
file(GLOB TEXTURE "SegoSystem/Core/Texture2D/*.cpp")


#添加OBJECT
file(GLOB OBJECT "SegoSystem/Object/*.cpp")
#添加Component
file(GLOB COMPONENT "SGComponent/*.cpp")
#添加Resource
file(GLOB RESOURCE "Resource/*.cpp")



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
list(APPEND DATA_LIST ${MESH})
list(APPEND DATA_LIST ${MATERIAL})
list(APPEND DATA_LIST ${COMPONENT})
list(APPEND DATA_LIST ${OBJECT})
list(APPEND DATA_LIST ${TEXTURE})
list(APPEND DATA_LIST ${RESOURCE})


#add exe
add_executable(editor  ${SRC_LIST} ${DATA_LIST})

#add link
target_link_libraries(editor PRIVATE imgui glfw3 rttr_core freetype lua assimp-5)



