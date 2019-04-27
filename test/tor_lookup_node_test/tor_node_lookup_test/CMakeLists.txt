cmake_minimum_required (VERSION 2.6)
project(tor_node_lookup_test)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})

set ( RESOURCES
  resource.qrc
)

find_package(CURL REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)

include_directories(${QT_INCLUDES} ../../../)

QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )

set(tor_node_lookup_test_SOURCES "main.cpp" "test_node_lookup.h" "test_node_lookup.cpp" "../../../node_lookup.h" "../../../node_lookup.cpp" "../../../node_lookup_interface.h" ${RES_SOURCES})

add_executable(tor_node_lookup_test ${tor_node_lookup_test_SOURCES})
target_link_libraries(tor_node_lookup_test Qt5::Core Qt5::Network GTest::GTest GTest::Main)
