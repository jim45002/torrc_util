cmake_minimum_required (VERSION 2.6)
project(torrc_utility)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set ( RESOURCES
  gui_resources.qrc
)

find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${QT_INCLUDES})

QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )

set(torrc_utility_SOURCES "main.cpp" "mainwindow.cpp" "tor_options_dialog.cpp" "tor_config_options.cpp" "tor_options_factory.cpp" "tor_options_dialog_factory.cpp" "node_lookup.cpp" "coordinator.cpp" "node_lookup.h" "node_lookup_interface.h" "mainwindow.h" "tor_options_dialog.h" "tor_config_options.h" "tor_config_options_interface.h" "main_window_interface.h" "tor_options_dialog_interface.h" "coordinator.h" "tor_options_factory.h" "tor_options_dialog_factory.h" "coordinator.h" "mainwindow.h" "main_window_interface.h" "tor_config_options_interface.h" "tor_config_options.h" "tor_options_dialog_factory.h" "tor_options_dialog_interface.h" "tor_options_dialog.h" "tor_options_factory.h" "mainwindow.ui" "tor_options.ui" ${RES_SOURCES})

add_executable(torrc_utility ${torrc_utility_SOURCES})
target_link_libraries(torrc_utility Qt5::Core Qt5::Gui Qt5::Widgets)
