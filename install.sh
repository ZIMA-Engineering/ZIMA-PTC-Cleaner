#!/bin/sh

PREFIX=/usr

cp bin/zima-ptc-cleaner $PREFIX/bin/zima-ptc-cleaner
mkdir $PREFIX/share/zima-ptc-cleaner
cp src/ptc-cleaner_sk.qm $PREFIX/share/zima-ptc-cleaner
cp src/ptc-cleaner_cs.qm $PREFIX/share/zima-ptc-cleaner
cp zima-ptc-cleaner.desktop `kde-config --prefix`/share/apps/konqueror/servicemenus/

echo "Done."
