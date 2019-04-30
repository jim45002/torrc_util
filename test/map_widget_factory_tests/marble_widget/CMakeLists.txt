cmake_minimum_required (VERSION 2.6)
project(map_widget)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set ( RESOURCES
  resources.qrc
)

find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${QT_INCLUDES} /usr/include ../../../map_widget_factory)

QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )

set(map_widget_SOURCES "main.cpp" "mainwindow.cpp" "mainwindow.ui" ${RES_SOURCES})

add_executable(map_widget ${map_widget_SOURCES})
target_link_libraries(map_widget Qt5::Core Qt5::Gui Qt5::Widgets map_widget_factory)
