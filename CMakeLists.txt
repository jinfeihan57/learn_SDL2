
cmake_minimum_required(VERSION 3.16.3)
project(hello_world)

enable_testing()

option(BUILD_DEBUG "Builds debug!!!" OFF)
if(BUILD_DEBUG)
    add_compile_options("-g")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "-mwindows")
else()
    message(STATUS "OS: "${CMAKE_SYSTEM_NAME})
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")

set(HELLO_SDL_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(HELLO_SDL_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})

add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/hello_SDL2.cpp)

find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
if (${SDL2_FOUND} AND ${SDL2_image_FOUND} AND ${SDL2_ttf_FOUND})
# TODO SDL2_image find
    message(STATUS ${SDL2_INCLUDE_DIRS})
    message(STATUS ${SDL2_STATIC_LIBRARIES})
    message(STATUS ${SDL2_INCLUDE_DIRS})
    target_include_directories(main PUBLIC ${SDL2_INCLUDE_DIR})
    target_link_libraries(main PUBLIC ${SDL2_STATIC_LIBRARIES} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL-release-2.28.3)
    set(BUILD_SHARED_LIBS OFF)
    set(SDL2IMAGE_SAMPLES OFF)
    set(SDL2IMAGE_INSTALL OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL_image-release-2.6.3)
    set(SDL2TTF_VENDORED TRUE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL_ttf-release-2.20.2)
    set(SDL2MIXER_VENDORED ON)
    set(SDL2MIXER_INSTALL OFF)
    set(SDL2MIXER_DEPS_SHARED OFF) #SDL2MIXER_DEPS_SHARED 关闭动态加载能力，采用链接的方式依赖库
    set(SDL2MIXER_MOD OFF) # 关闭SDL2MIXER_DEPS_SHARED导致MOD能力编译失败，考虑到MOD能力使用较少，因此也关闭
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/SDL_mixer-release-2.6.3)
    target_link_libraries(main PUBLIC SDL2::SDL2-static SDL2::SDL2main SDL2_image SDL2_ttf SDL2_mixer)
endif()

# 设置 编译测试选项
option(BUILD_TEST "Builds test!!!" OFF)
if(BUILD_TEST)
    add_subdirectory(tests)
endif()
