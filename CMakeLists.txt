cmake_minimum_required(VERSION 2.8)
project(lch)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

#开启编译详情
set(CMAKE_VERBOSE_MAKEFILE ON)
#自定义编译参数
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

set(LIB_SRC
    lch/log.cc
    lch/util.cc
    lch/config.cc
    )

add_library(lch SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(lch) #重定义__FILE__这个宏
target_link_libraries(lch yaml-cpp)
#add_library(lch_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (lch_static PROPERTIES OUTPUT_NAME "lch")

add_executable(test tests/test.cc)
force_redefine_file_macro_for_sources(test) #重定义__FILE__这个宏
add_dependencies(test lch)
target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/)
target_link_libraries(test lch)

add_executable(test_config tests/test_config.cc)
force_redefine_file_macro_for_sources(test_config) #重定义__FILE__这个宏
add_dependencies(test_config lch)
target_include_directories(test_config PUBLIC ${CMAKE_SOURCE_DIR}/)
target_link_libraries(test_config lch yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

