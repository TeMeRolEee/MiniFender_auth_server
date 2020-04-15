cmake_minimum_required(VERSION 3.15)
project(MiniFender_auth_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC)
set(CMAKE_AUTORCC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(auth_settings.ini settings/auth_settings.ini COPYONLY)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(MiniFender_auth_server main.cpp core.cpp core.h clihandler.cpp clihandler.h authserver.cpp authserver.h)

target_link_libraries(MiniFender_auth_server Qt5::Core Qt5::Network)
