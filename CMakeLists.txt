cmake_minimum_required(VERSION 3.16.3)

project(submitty_count_ts)

add_library(tree-sitter-python include/tree-sitter-python/src/parser.c
                               include/tree-sitter-python/src/scanner.cc)

add_library(tree-sitter-c include/tree-sitter-c/src/parser.c)

add_executable(submitty_count_ts src/count.cpp src/parser.cpp src/utils.cpp)

add_executable(submitty_diagnostics_ts src/diagnostics.cpp src/parser.cpp
                                       src/utils.cpp)

include_directories(include/tree-sitter/lib/include)

target_link_libraries(
  submitty_count_ts tree-sitter-python tree-sitter-c
  ${PROJECT_SOURCE_DIR}/include/tree-sitter/libtree-sitter.a)

target_link_libraries(
  submitty_diagnostics_ts tree-sitter-python tree-sitter-c
  ${PROJECT_SOURCE_DIR}/include/tree-sitter/libtree-sitter.a)

target_include_directories(submitty_count_ts
                           PUBLIC include/tree-sitter/lib/include)

target_include_directories(submitty_diagnostics_ts
                           PUBLIC include/tree-sitter/lib/include)
