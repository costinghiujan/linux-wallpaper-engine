cmake_minimum_required(VERSION 3.16)

project(FedoraWallpaper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Widgets)

qt_add_executable(FedoraWallpaper
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
)

target_link_libraries(FedoraWallpaper PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
)