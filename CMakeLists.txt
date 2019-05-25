cmake_minimum_required (VERSION 2.6)
project(torrc_util)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(SOURCE_DIR ./src)

set ( RESOURCES
  ./resources/gui_resources.qrc
)

find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${QT_INCLUDES} ./libs/map_widget_factory)

QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )

set(torrc_utility_SOURCES "./src/main.cpp"
                          "./src/mainwindow.cpp"
                          "./src/tor_options_dialog.cpp"
                          "./src/tor_config_options.cpp"
                          "./src/tor_options_factory.cpp"
                          "./src/tor_options_dialog_factory.cpp"
                          "./src/node_lookup.cpp"
                          "./src/coordinator.cpp"
                          "./src/node_lookup.h"
                          "./src/node_lookup_interface.h"
                          "./src/mainwindow.h"
                          "./src/tor_options_dialog.h"
                          "./src/tor_config_options.h"
                          "./src/tor_config_options_interface.h"
                          "./src/main_window_interface.h"
                          "./src/tor_options_dialog_interface.h"
                          "./src/coordinator.h"
                          "./src/tor_options_factory.h"
                          "./src/tor_options_dialog_factory.h"
                          "./src/coordinator.h"
                          "./src/mainwindow.h"
                          "./src/main_window_interface.h"
                          "./src/tor_config_options_interface.h"
                          "./src/tor_config_options.h"
                          "./src/tor_options_dialog_factory.h"
                          "./src/tor_options_dialog_interface.h"
                          "./src/tor_options_dialog.h"
                          "./src/tor_options_factory.h"
                          "./src/mainwindow.ui"
                          "./src/tor_options.ui"
                          ${RES_SOURCES})

add_executable(torrc_util ${torrc_utility_SOURCES})
target_link_libraries(torrc_util Qt5::Core Qt5::Gui Qt5::Widgets map_widget_factory)

include(GNUInstallDirs)

INSTALL(TARGETS torrc_util RUNTIME DESTINATION /usr/local/bin)
