#!/bin/bash

CODA_INSTALL="../../multigrid/comma-0.1/thirdparty/comma/src/"
CODA_HEADER="../../multigrid/comma-0.1/thirdparty/comma/include/Comma/"

echo "Removing previous version"
rm -r -v ${CODA_INSTALL}*
rm -r -v ${CODA_HEADER}*

echo "Installing actual version"
rsync -av CoMMA_lib/*.cpp ${CODA_INSTALL} --exclude=CoMMA.cpp
rsync -av CoMMA_lib/*.h ${CODA_HEADER}

