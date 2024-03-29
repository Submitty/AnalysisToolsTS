cmake_minimum_required(VERSION 3.16.3)
set (CMAKE_CXX_STANDARD 14)

project(analysis_tools_ts)

set(JSONCODE include/json/include/)

if(JSONDIR)
  set(JSONCODE ${JSONDIR})
endif()

add_library(tree-sitter-python include/tree-sitter-python/src/parser.c
                               include/tree-sitter-python/src/scanner.c)

add_library(tree-sitter-cpp include/tree-sitter-cpp/src/parser.c
                            include/tree-sitter-cpp/src/scanner.c)

add_library(tree-sitter-c include/tree-sitter-c/src/parser.c)

add_library(tree-sitter-java include/tree-sitter-java/src/parser.c)

link_libraries(tree-sitter-python tree-sitter-c tree-sitter-cpp tree-sitter-java
               ${PROJECT_SOURCE_DIR}/include/tree-sitter/libtree-sitter.a)

add_executable(submitty_count_ts src/count.cpp src/parser.cpp src/utils.cpp)

add_executable(submitty_diagnostics_ts src/diagnostics.cpp src/parser.cpp
                                       src/utils.cpp)

include_directories(include/tree-sitter/lib/include)

target_include_directories(submitty_diagnostics_ts PUBLIC ${JSONCODE})
