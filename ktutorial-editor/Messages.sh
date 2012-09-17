#!/bin/sh

BASEDIR="src/" # root of translatable sources
PROJECT="ktutorial-editor" # project name

# we use simple sorting to make sure the lines do not jump around too much from system to system
find ${BASEDIR} -name '*.rc' -o -name '*.ui' -o -name '*.kcfg' | sort > rcfiles.list
xargs --arg-file=rcfiles.list extractrc > rc.cpp

# see above on sorting
find ${BASEDIR} -name '*.cpp' -o -name '*.h' -o -name '*.c' -o -name '*.js' -o -name '*.py' -o -name '*.rb' | sort > infiles.list
echo "rc.cpp" >> infiles.list
$XGETTEXT --files-from=infiles.list -o $podir/${PROJECT}.pot

rm -f rcfiles.list rc.cpp infiles.list
