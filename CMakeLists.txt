cmake_minimum_required(VERSION 3.16.3)
project(analysis_tools_ts C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Give extracted files the time of extraction as their timestamp
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

# Dependencies are downloaded as source archives of pinned commits instead of
# git clones: GIT_SHALLOW cannot fetch arbitrary commits, and full clones of
# these repositories take minutes.  When changing a commit, update its hash
# too, e.g. with `curl -sL <archive URL> | sha256sum`.

FetchContent_Declare(
    tree-sitter
    URL      https://github.com/tree-sitter/tree-sitter/archive/0535b0ca378a3f13a2b469f6c090a0c1b90904b7.tar.gz
    URL_HASH SHA256=bc38443b2b4bba754e68efb0a6159ed4a0ff0f84064e25125bf87de79b7c9dca
)

FetchContent_GetProperties(tree-sitter)
if(NOT tree-sitter_POPULATED)
    FetchContent_Populate(tree-sitter)

    # Build the core library from the fetched source
    add_library(tree-sitter STATIC ${tree-sitter_SOURCE_DIR}/lib/src/lib.c)
    target_include_directories(tree-sitter PUBLIC ${tree-sitter_SOURCE_DIR}/lib/include)
endif()

function(fetch_and_build_ts_parser PARSER_NAME REPO_URL COMMIT SHA256)
    FetchContent_Declare(
        ${PARSER_NAME}
        URL      ${REPO_URL}/archive/${COMMIT}.tar.gz
        URL_HASH SHA256=${SHA256}
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
  https://github.com/tree-sitter/tree-sitter-python
  26855eabccb19c6abf499fbc5b8dc7cc9ab8bc64
  43e71c4d0cbbd2350e83e0b2da3558eab09b8dc78153a8990508b0c9eec754e6
)

fetch_and_build_ts_parser(
  tree-sitter-c
  https://github.com/tree-sitter/tree-sitter-c
  ae19b676b13bdcc13b7665397e6d9b14975473dd
  ac9cf7b72ec84f08fdea5600623d9a38f32c78e58b9170846b278431f856c5d6
)

fetch_and_build_ts_parser(
  tree-sitter-cpp
  https://github.com/tree-sitter/tree-sitter-cpp
  8b5b49eb196bec7040441bee33b2c9a4838d6967
  a12b464b8c89d7c1843849bfebb0b5807980555351d60598b92939233abff842
)

fetch_and_build_ts_parser(
  tree-sitter-java
  https://github.com/tree-sitter/tree-sitter-java
  e10607b45ff745f5f876bfa3e94fbcc6b44bdc11
  95a46b6b7b545b48cd6c3a0d5b1014fed271a63a71164cdc4382d9d441e3609f
)

FetchContent_Declare(
    nlohmann_json
    URL      https://github.com/nlohmann/json/archive/9a737481aed085fd289f82dff1fa8c3c66627a7e.tar.gz
    URL_HASH SHA256=f2a3a52aa953de57a3a81ceab496a032c376f4977cf05533b9f80752c7a3d998
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