cmake_minimum_required(VERSION 3.16.0)

project(Gen4ID VERSION 2.0.0 LANGUAGES CXX)
add_compile_definitions(VERSION="2.0.0")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(PROJECT_UI
    Forms/MainWindow.ui
)

set(PROJECT_SOURCES  
    Core/IDResult.cpp
    Core/IDSearcher.cpp
    Forms/Controls/TextBox.cpp
    Forms/MainWindow.cpp
    Model/IDModel.cpp
    main.cpp
)

if (WIN32)
    add_executable(Gen4ID
        WIN32
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
    )
elseif (APPLE)
    add_executable(Gen4ID
        MACOSX_BUNDLE
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
    )
else ()
    add_executable(Gen4ID
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
    )
endif ()

target_link_libraries(Gen4ID PRIVATE Qt6::Core Qt6::Widgets)
