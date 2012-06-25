#!/bin/sh

PREFIX=/usr

rm $PREFIX/bin/zima-ptc-cleaner
rm $PREFIX/share/zima-ptc-cleaner/ptc-cleaner_sk.qm
rm $PREFIX/share/zima-ptc-cleaner/ptc-cleaner_cs.qm
rmdir $PREFIX/share/zima-ptc-cleaner
rm -f `kde-config --prefix`/share/apps/konqueror/servicemenus/zima-ptc-cleaner.desktop
echo "Done."
