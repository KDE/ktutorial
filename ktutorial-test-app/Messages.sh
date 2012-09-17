#!/bin/sh

PROJECT="ktutorial-test-app" # project name

# we use simple sorting to make sure the lines do not jump around too much from system to system
find . -name '*.rc' -o -name '*.ui' -o -name '*.kcfg' | sort > rcfiles.list
xargs --arg-file=rcfiles.list extractrc > rc.cpp

# see above on sorting
find . -name '*.cpp' -o -name '*.h' -o -name '*.c' -o -name '*.js' -o -name '*.py' -o -name '*.rb' | sort > infiles.list
echo "rc.cpp" >> infiles.list
$XGETTEXT --files-from=infiles.list -o $podir/${PROJECT}.pot

rm -f rcfiles.list infiles.list rc.cpp 
