#!/bin/sh

BASEDIR="src/" # root of translatable sources
PROJECT="ktutorial" # project name

# we use simple sorting to make sure the lines do not jump around too much from system to system
find ${BASEDIR} -name '*.rc' -o -name '*.ui' -o -name '*.kcfg' | sort > rcfiles.list
xargs --arg-file=rcfiles.list $EXTRACTRC > rc.cpp

# see above on sorting
find ${BASEDIR} -name '*.cpp' -o -name '*.h' -o -name '*.c' | sort > infiles.list
echo "rc.cpp" >> infiles.list
$XGETTEXT --files-from=infiles.list -o $podir/${PROJECT}.pot

rm -f rcfiles.list infiles.list rc.cpp 
