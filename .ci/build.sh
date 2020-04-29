#!/bin/bash

case $OS in
  linux)
  {
    source /opt/qt5*/bin/qt5*-env.sh
    qmake Gen4IDs.pro
    make -j $(nproc)
  } ;;  
  macOS)
  {
    qmake Gen4IDs.pro
    make -j $(sysctl -n hw.physicalcpu)
  } ;;
esac