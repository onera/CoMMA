#!/bin/bash

CODA_INSTALL="../../multigrid/algorithm_comma_brench/thirdparty/comma/src/"
CODA_HEADER="../../multigrid/algorithm_comma_brench/thirdparty/comma/include/Comma/"

echo "Removing previous version"
rm -r -v ${CODA_INSTALL}*
rm -r -v ${CODA_HEADER}*

echo "Installing actual version"
cp -r -v CoMMA_lib/*.cpp ${CODA_INSTALL}
cp -r -v CoMMA_lib/*.h ${CODA_HEADER}

