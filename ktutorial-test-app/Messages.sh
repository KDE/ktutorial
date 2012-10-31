#!/bin/sh

$EXTRACTRC `find . -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.h -o -name \*.js -o -name \*.py -o -name \*.rb` -o $podir/ktutorial-test-app.pot

# extract-messages.sh should take care of removing rc.cpp, but just in case
rm -f rc.cpp
