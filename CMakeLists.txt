cmake_minimum_required(VERSION 3.16.3)
project(analysis_tools_ts C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    tree-sitter
    GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter.git
    GIT_TAG        0535b0ca378a3f13a2b469f6c090a0c1b90904b7
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(tree-sitter)
if(NOT tree-sitter_POPULATED)
    FetchContent_Populate(tree-sitter)

    # Build the core library from the fetched source
    add_library(tree-sitter STATIC ${tree-sitter_SOURCE_DIR}/lib/src/lib.c)
    target_include_directories(tree-sitter PUBLIC ${tree-sitter_SOURCE_DIR}/lib/include)
endif()

function(fetch_and_build_ts_parser PARSER_NAME GIT_URL GIT_COMMIT)
    FetchContent_Declare(
        ${PARSER_NAME}
        GIT_REPOSITORY ${GIT_URL}
        GIT_TAG        ${GIT_COMMIT}
        GIT_SHALLOW    TRUE
    )
    FetchContent_GetProperties(${PARSER_NAME})
    if(NOT ${PARSER_NAME}_POPULATED)
        FetchContent_Populate(${PARSER_NAME})

        # Collect sources
        set(SOURCES ${${PARSER_NAME}_SOURCE_DIR}/src/parser.c)
        if(EXISTS ${${PARSER_NAME}_SOURCE_DIR}/src/scanner.c)
            list(APPEND SOURCES ${${PARSER_NAME}_SOURCE_DIR}/src/scanner.c)
        elseif(EXISTS ${${PARSER_NAME}_SOURCE_DIR}/src/scanner.cc)
            list(APPEND SOURCES ${${PARSER_NAME}_SOURCE_DIR}/src/scanner.cc)
        endif()

        add_library(${PARSER_NAME} STATIC ${SOURCES})
        target_include_directories(${PARSER_NAME} PRIVATE ${${PARSER_NAME}_SOURCE_DIR}/src)
        target_link_libraries(${PARSER_NAME} PRIVATE tree-sitter)
    endif()
endfunction()

# Fetch each language.  Uses the latest commit as of 03-29-2026

fetch_and_build_ts_parser(
  tree-sitter-python
  https://github.com/tree-sitter/tree-sitter-python.git
  26855eabccb19c6abf499fbc5b8dc7cc9ab8bc64
)

fetch_and_build_ts_parser(
  tree-sitter-c
  https://github.com/tree-sitter/tree-sitter-c.git
  ae19b676b13bdcc13b7665397e6d9b14975473dd
)

fetch_and_build_ts_parser(
  tree-sitter-cpp
  https://github.com/tree-sitter/tree-sitter-cpp.git
  8b5b49eb196bec7040441bee33b2c9a4838d696
)

fetch_and_build_ts_parser(
  tree-sitter-java
  https://github.com/tree-sitter/tree-sitter-java.git
  e10607b45ff745f5f876bfa3e94fbcc6b44bdc11
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        9a737481aed085fd289f82dff1fa8c3c66627a7e
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

set(COMMON_LIBS
    tree-sitter
    tree-sitter-python
    tree-sitter-c
    tree-sitter-cpp
    tree-sitter-java
)

# Submitty Count
add_executable(submitty_count_ts
    src/count.cpp
    src/parser.cpp
    src/utils.cpp
)
target_link_libraries(submitty_count_ts PRIVATE ${COMMON_LIBS})

# Submitty Diagnostics
add_executable(submitty_diagnostics_ts
    src/diagnostics.cpp
    src/parser.cpp
    src/utils.cpp
)
target_link_libraries(submitty_diagnostics_ts PRIVATE
    ${COMMON_LIBS}
    nlohmann_json::nlohmann_json
)