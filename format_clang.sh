#!/usr/bin/env bash
git ls-tree --name-only -r HEAD CoMMA_lib tests | grep ".*\.\(h\|cpp\)" | xargs clang-format --style=file -i
