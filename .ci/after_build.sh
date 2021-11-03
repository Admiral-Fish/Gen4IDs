#!/bin/bash

case $OS in
  linux)
  {
    mv build/Release/Gen4IDs .
    zip -r Gen4IDs-linux.zip Gen4IDs
    sha256sum Gen4IDs-linux.zip > Gen4IDs-linux.zip.sha256
  } ;;  
  macOS)
  {
    mv build/Release/Gen4IDs.app .
    PATH=$PATH:$HOME/Qt/6.1/macos/bin macdeployqt Gen4IDs.app -dmg -verbose=2
    zip -r Gen4IDs-macOS.zip Gen4IDs.dmg
    shasum -a 256 Gen4IDs-macOS.zip > Gen4IDs-macOS.zip.sha256
  } ;;
esac