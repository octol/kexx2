#!/usr/bin/env bash
VERSION=$(grep " VERSION " config.h | cut -d' ' -f3 | sed s/\"//g | sed s/\\./_/g)
DIR=kexx2-$VERSION-win-x86
mkdir -p $DIR
cp -a src/kexx2.exe $DIR
cp -a data $DIR
cp -a README $DIR/readme.txt
cp -a AUTHORS $DIR/authors.txt
cp -a ChangeLog $DIR/changelog.txt
cp -a COPYING $DIR/copying.txt
zip -r $DIR.zip $DIR
