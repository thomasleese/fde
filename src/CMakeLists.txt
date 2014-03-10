set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

qt5_wrap_ui(UI_HEADERS mainwindow.ui instructionswidget.ui loadprogramdialogue.ui)
qt5_add_resources(RESOURCE_SOURCES resources.qrc)

add_executable(fde main.cpp mainwindow.cpp component.cpp instructionswidget.cpp loadprogramdialogue.cpp ${UI_HEADERS} ${RESOURCE_SOURCES})
qt5_use_modules(fde Widgets)
