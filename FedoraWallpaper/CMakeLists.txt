cmake_minimum_required(VERSION 3.16)

project(FedoraWallpaper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 1. Find Dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Widgets QuickWidgets WaylandClient ShaderTools)
find_package(X11 REQUIRED)

# 2. CREATE THE APP FIRST (Move this up!)
qt_add_executable(FedoraWallpaper
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    resources.qrc
)

# 3. NOW Configure Shaders (Because "FedoraWallpaper" exists now)
qt_add_shaders(FedoraWallpaper "shaders"
    PREFIX "/"
    FILES
        assets/shaders/water.frag
)

# 4. Link Libraries
target_link_libraries(FedoraWallpaper PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
    Qt6::QuickWidgets
    Qt6::WaylandClient
    Qt6::ShaderTools
    X11::X11
)