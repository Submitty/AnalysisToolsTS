cmake_minimum_required(VERSION 3.16.3)

project(submitty_count_ts)

find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp vendor/tree-sitter-python/src/parser.c src/counter.cpp src/parser.cpp src/utils.cpp vendor/tree-sitter-python/src/scanner.cc)

include_directories(vendor/tree-sitter/lib/include ${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/vendor/tree-sitter/libtree-sitter.a ${Boost_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC vendor/tree-sitter/lib/include)
