#!/usr/bin/env bash
git ls-tree --name-only -r HEAD src include/CoMMA tests | grep ".*\.\(h\|cpp\)" | xargs clang-format --style=file -i
