#!/bin/sh

$EXTRACTRC `find src -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
# rc.cpp must be included explicitly, as it is created in the base directory,
# but find searches only in the src subdirectory
$XGETTEXT `find src -name \*.cpp -o -name \*.h` rc.cpp -o $podir/ktutorial.pot

# extract-messages.sh should take care of removing rc.cpp, but just in case
rm -f rc.cpp
