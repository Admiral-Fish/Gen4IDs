#!/bin/bash

case $OS in
  linux)
  {
    mv build/Source/Forms/Gen4ID .
    zip -r Gen4ID-linux.zip Gen4ID
    sha256sum Gen4ID-linux.zip > Gen4ID-linux.zip.sha256
  } ;;  
  macOS)
  {
    mv build/Source/Forms/Gen4ID.app .
    PATH=$PATH:$HOME/Qt/6.1/macos/bin macdeployqt Gen4ID.app -dmg -verbose=2
    zip -r Gen4ID-macOS.zip Gen4ID.dmg
    shasum -a 256 Gen4ID-macOS.zip > Gen4ID-macOS.zip.sha256
  } ;;
esac