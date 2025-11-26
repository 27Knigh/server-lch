cmake_minimum_required(VERSION 3.10)
project(lch)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

#开启编译详情
set(CMAKE_VERBOSE_MAKEFILE ON)
#自定义编译参数
# 改进的方式
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -O0 -ggdb -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")
set(CMAKE_CXX_STANDARD 11)

# 查找 pthreads
find_package(Threads REQUIRED)
# 查找 yaml-cpp
find_package(yaml-cpp REQUIRED CONFIG)

set(LIB_SRC
    lch/log.cc
    lch/util.cc
    lch/config.cc
    lch/thread.cc
    )



add_library(lch SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(lch) #重定义__FILE__这个宏


if (TARGET yaml-cpp::yaml-cpp)
  target_link_libraries(lch 
    PUBLIC 
        Threads::Threads
        yaml-cpp::yaml-cpp)
elseif (TARGET yaml-cpp)
  target_link_libraries(lch 
    PUBLIC 
        Threads::Threads
        yaml-cpp)
else()
  message(FATAL_ERROR "yaml-cpp found but expected target not exported.")
endif()

message(STATUS "CMAKE_CURRENT_SOURCE_DIR is " ${CMAKE_CURRENT_SOURCE_DIR})


#设置包含路径
target_include_directories(lch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test tests/test.cc)
force_redefine_file_macro_for_sources(test) #重定义__FILE__这个宏
#测试程序只需要连接主库，依赖会自动传递
target_link_libraries(test PRIVATE lch)

add_executable(test_config tests/test_config.cc)
force_redefine_file_macro_for_sources(test_config) #重定义__FILE__这个宏
target_link_libraries(test_config PRIVATE lch)

add_executable(test_thread tests/test_thread.cc)
force_redefine_file_macro_for_sources(test_thread) #重定义__FILE__这个宏
target_link_libraries(test_thread PRIVATE lch)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

