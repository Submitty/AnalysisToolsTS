cmake_minimum_required(VERSION 3.16.3)

project(submitty_count_ts)

add_executable(${PROJECT_NAME} main.cpp vendor/tree-sitter-python/src/parser.c vendor/tree-sitter-python/src/scanner.cc)

include_directories(vendor/tree-sitter/lib/include)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/vendor/tree-sitter/libtree-sitter.a)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/tree-sitter/lib/include)
