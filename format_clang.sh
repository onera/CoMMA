#!/usr/bin/env bash
git ls-tree --name-only -r HEAD src include/CoMMA tests config_files | grep ".*\.\(h\|cpp\)\(\.in\)\?" | xargs clang-format --style=file -i
