QT += widgets
CONFIG += c++1z

TARGET = Gen4IDs
TEMPLATE = app
VERSION = 2.0.0

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEPRECATED_WARNINGS

FORMS += \
    Forms/MainWindow.ui

HEADERS += \
    Core/IDSearcher.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/MainWindow.hpp \
    Model/IDModel.hpp \
    Core/IDResult.hpp

SOURCES += \
    Core/IDSearcher.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/MainWindow.cpp \
    main.cpp \
    Model/IDModel.cpp \
    Core/IDResult.cpp
