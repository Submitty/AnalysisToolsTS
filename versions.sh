#!/usr/bin/env bash
########################################################################

# These variables specify the version of these dependencies that we
# want. This affects both checking out the repo as well as installation
# of built artificats.

# If tree sitter in the future has more consistant releases, or better ways 
# of locking the version, adjust this to be versions instead of commits

# TREE-SITTER REPOS
# Python -- Merge pull request #193 from tree-sitter/tausbn/... -- Feb 28, 23
export tree_sitter_python_hash=62827156d01c74dc1538266344e788da74536b8a
# Tree-Sitter - Avoid unused value warning from array_pop -- Jun 14, 23
export tree_sitter_hash=d0029a15273e526925a764033e9b7f18f96a7ce5
# cpp -- Merge pull request #209 from amaanq/rewrite-it-in-c -- Jun 18, 23
export tree_sitter_cpp_hash=2e29f23abcb5f92ddffb22de8b292f09ed78db01
# C -- Merge pull request #141 from JasonBrownDeveloper/offsetof -- Jun 13, 23
export tree_sitter_c_hash=f1b2a0b2102ff001a4ed9a26cd9a9405d09c590c
# Java -- Merge pull request #144 from stelf/patch-1 -- April 19, 23
export tree_sitter_java_hash=c194ee5e6ede5f26cf4799feead4a8f165dcf14d