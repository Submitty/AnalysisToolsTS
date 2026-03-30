#!/usr/bin/env bash

set -e

if [[ "$UID" -ne "0" ]] ; then
    echo "ERROR: This script must be run as root"
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

BUILD_DIR="${INSTALLATION_DIR}/build"
mkdir -p "${BUILD_DIR}"

cmake -S "${INSTALLATION_DIR}" -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" --parallel "$(nproc)"

if [ $# -eq 0 ]; then
    chown -R root:root "${INSTALLATION_DIR}"
    chmod -R 755 "${INSTALLATION_DIR}"
fi

echo "Done setting up AnalysisToolsTS"
