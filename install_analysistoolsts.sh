#!/usr/bin/env bash

########################################################################################################################
########################################################################################################################
# this script must be run by root or sudo
if [[ "$UID" -ne "0" ]] ; then
    echo "ERROR: This script must be run by root or sudo"
    exit 1
fi

VARS="$(dirname "$0")"/submitty.sh

if [ "$1" == "local" ] || [ $# -gt 1 ]; then
    VARS="$(dirname "$0")"/local.sh
fi

source "${VARS}"

echo -e "Installing AnalysisToolsTS... "

mkdir -p "${INSTALLATION_DIR}"

# Copy cloned files to AnalysisToolsTS directory
rsync -rtz "${REPO_DIR}" "${INSTALLATION_DIR}"

mkdir -p "${VENDOR_DIR}"

########################################################################

# Clone the tree-sitter repos
repos=( tree-sitter tree-sitter-python tree-sitter-c )

for repo in "${repos[@]}"
do
    dir="${VENDOR_DIR}"/"${repo}"

    echo "clone or update ${repo}... "

    if [ -d "${dir}" ]; then
        echo "pulling changes ..."
        # IF THE REPO ALREADY EXISTS...
        pushd "${dir}" || exit

        # PULL CHANGES
        git pull
        popd || exit

    else
        # THE REPO DID NOT EXIST
        echo "the repository did not previously exist cloning... "
        pushd "${VENDOR_DIR}" || exit
        git clone --depth 1 "https://github.com/tree-sitter/${repo}" 2 || exit
        popd || exit

    fi
done

########################################################################

# boost
apt-get install -y libboost-all-dev

# build tree sitter library
pushd "${VENDOR_DIR}"/tree-sitter || exit

make

popd || exit

echo "building submitty_count_ts ..."

# Compile the project
mkdir -p "${INSTALLATION_DIR}/build"

cmake -S "${INSTALLATION_DIR}" -B "${INSTALLATION_DIR}/build"

pushd "${INSTALLATION_DIR}/build" || exit

make

popd || exit

# # change permissions
chown -R root:root "${INSTALLATION_DIR}"
chmod -R 755 "${INSTALLATION_DIR}"

echo "Done setting up AnalysisToolsTS"
