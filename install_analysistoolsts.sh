#!/usr/bin/env bash

set -e

########################################################################################################################
########################################################################################################################
# this script must be run by root or sudo
if [[ "$UID" -ne "0" ]] ; then
    echo "ERROR: This script must be run by root or sudo"
    exit 1
fi

VARS="$(dirname "$0")"/submitty.sh

if [ $# -gt 0 ] && [ "$1" == "local" ]; then
    VARS="$(dirname "$0")"/local.sh
fi

source "${VARS}"

echo -e "Installing AnalysisToolsTS... "

mkdir -p "${INSTALLATION_DIR}"

# Copy cloned files to AnalysisToolsTS directory
if [ $# -eq 0 ]; then
    rsync -rtz "${REPO_DIR}/src" "${REPO_DIR}/CMakeLists.txt" "${INSTALLATION_DIR}"
fi

mkdir -p "${INCLUDE_DIR}"

########################################################################

# Clone the tree-sitter repos
repos=( tree-sitter tree-sitter-python tree-sitter-c tree-sitter-cpp)

for repo in "${repos[@]}"
do
    dir="${INCLUDE_DIR}"/"${repo}"

    echo "clone or update ${repo}... "

    if [ -d "${dir}" ]; then
        echo "pulling changes ..."
        # IF THE REPO ALREADY EXISTS...
        pushd "${dir}"

        CURRENT_BRANCH=$(git branch --show-current)

        # PULL CHANGES
        git fetch
        git reset --hard HEAD
        git merge origin/"${CURRENT_BRANCH}"
        popd

    else
        # THE REPO DID NOT EXIST
        echo "the repository did not previously exist cloning... "
        pushd "${INCLUDE_DIR}"
        git clone --depth 1 "https://github.com/tree-sitter/${repo}"
        popd

    fi
done

# CHECKOUT & INSTALL THE NLOHMANN C++ JSON LIBRARY
# If we don't already have a copy of this repository, check it out, only for local development
if [ $# -gt 0 ] && [ ! -d "${NLOHMANN_DIR}" ]; then
    git clone --depth 1 "https://github.com/nlohmann/json.git" "${NLOHMANN_DIR}"
fi

########################################################################

# build tree sitter library
pushd "${INCLUDE_DIR}"/tree-sitter

make

popd

echo "building submitty_count_ts ..."

# Compile the project
mkdir -p "${INSTALLATION_DIR}/build"

cmake -S "${INSTALLATION_DIR}" -B "${INSTALLATION_DIR}/build" -DJSONDIR="${NLOHMANN_INCLUDE_DIR}"

pushd "${INSTALLATION_DIR}/build"

make

popd

# # change permissions
if [ $# -eq 0 ]; then
    chown -R root:root "${INSTALLATION_DIR}"
    chmod -R 755 "${INSTALLATION_DIR}"
fi

echo "Done setting up AnalysisToolsTS"
