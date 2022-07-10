cmake_minimum_required(VERSION 3.16.3)

project(submitty_count_ts)

add_library(tree-sitter-python include/tree-sitter-python/src/parser.c
                               include/tree-sitter-python/src/scanner.cc)

add_library(tree-sitter-c include/tree-sitter-c/src/parser.c)

add_executable(${PROJECT_NAME} src/main.cpp src/counter.cpp src/parser.cpp
                               src/utils.cpp)

include_directories(include/tree-sitter/lib/include)

target_link_libraries(
  ${PROJECT_NAME} tree-sitter-python tree-sitter-c
  ${PROJECT_SOURCE_DIR}/include/tree-sitter/libtree-sitter.a)

target_include_directories(${PROJECT_NAME}
                           PUBLIC include/tree-sitter/lib/include)
