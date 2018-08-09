#!/bin/bash

/usr/local/Cellar/qt/5.11.1/bin/macdeployqt Gen4IDs.app -dmg -verbose=2
zip -r Gen4IDs-macOS.zip Gen4IDs.dmg
