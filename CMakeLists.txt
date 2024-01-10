cmake_minimum_required(VERSION 2.8)
project(lch)

#开启编译详情
set(CMAKE_VERBOSE_MAKEFILE ON)
#自定义编译参数
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -o0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function ")


set(LIB_SRC
    lch/log.cc
    )

add_library(lch SHARED ${LIB_SRC})
#add_library(lch_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (lch_static PROPERTIES OUTPUT_NAME "lch")

add_executable(test tests/test.cc)
add_dependencies(test lch)
target_link_libraries(test lch)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

