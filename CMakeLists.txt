cmake_minimum_required(VERSION 2.8.9)
project(fde)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

qt5_wrap_ui(UI src/mainwindow.ui src/loadprogramdialogue.ui src/instructionswidget.ui)
qt5_add_resources(RES src/resources.qrc)

add_executable(fde src/main.cpp src/mainwindow.cpp src/component.cpp src/instructionswidget.cpp src/loadprogramdialogue.cpp ${UI} ${RES})
qt5_use_modules(fde Widgets Network)
