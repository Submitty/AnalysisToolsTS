#!/usr/bin/env bash



set -e

echo "Building AnalysisToolsTS... "

CUR_DIR=$(dirname "${0}")
BUILD_DIR=${CUR_DIR}/build
INCLUDE_DIR=${CUR_DIR}/include

mkdir -p "${BUILD_DIR}"
mkdir -p "${INCLUDE_DIR}"

########################################################################

# These variables specify the minimum version necessary for
# dependencies between versions.
source ${CUR_DIR}/versions.sh

########################################################################

# Clone the tree-sitter repos
repos=( tree-sitter tree-sitter-python tree-sitter-c tree-sitter-cpp tree-sitter-java)

for repo in "${repos[@]}"
do
    dir="${INCLUDE_DIR}/${repo}"

    echo "clone or update ${repo}... "

    if [ -d "${dir}" ]; then
        echo "pulling changes ..."
        # IF THE REPO ALREADY EXISTS...
        pushd "${dir}"

        # PULL CHANGES
        git fetch
        git reset --hard HEAD
        git merge "origin/${repo}_hash"

        popd
    else
        # THE REPO DID NOT EXIST
        echo "the repository did not previously exist cloning... "
        git clone --depth 1 "https://github.com/tree-sitter/${repo}" "${INCLUDE_DIR}/${repo}" --branch ${repo}_hash
    fi
done

# CHECKOUT & INSTALL THE NLOHMANN C++ JSON LIBRARY
echo "clone or update nlohmann"
if [ -d "${INCLUDE_DIR}/json" ]; then
  echo "pulling changes ..."
  pushd "${INCLUDE_DIR}/json"

  CURRENT_BRANCH=$(git branch --show-current)
  git fetch
  git reset --hard HEAD
  git merge "origin/${CURRENT_BRANCH}"
  popd
else
  git clone --depth 1 "https://github.com/nlohmann/json.git" "${INCLUDE_DIR}/json"
fi


########################################################################

# build tree sitter library
pushd "${INCLUDE_DIR}/tree-sitter"
make
popd

echo "building submitty_count_ts ..."

# Compile the project
cmake -S "${CUR_DIR}" -B "${BUILD_DIR}" -DJSONDIR="${INCLUDE_DIR}/json/include"

pushd "${BUILD_DIR}"
make
popd

echo "Done building AnalysisToolsTS"
