cmake_minimum_required(VERSION 3.16.3)

project(submitty_count_ts)

add_executable(${PROJECT_NAME} src/main.cpp include/tree-sitter-python/src/parser.c include/tree-sitter-c/src/parser.c src/counter.cpp src/parser.cpp src/utils.cpp include/tree-sitter-python/src/scanner.cc)

include_directories(include/tree-sitter/lib/include)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/include/tree-sitter/libtree-sitter.a)

target_include_directories(${PROJECT_NAME} PUBLIC include/tree-sitter/lib/include)
