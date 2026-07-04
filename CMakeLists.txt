cmake_minimum_required(VERSION 3.16)
project(mindev LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/settingsmanager.cpp
    src/settingsmanager.h
    src/settingsdialog.cpp
    src/settingsdialog.h
    src/stylemanager.cpp
    src/stylemanager.h
    src/mainwindow.cpp
    src/mainwindow.h
    src/tools/toolinterface.h
    src/tools/toolfactory.cpp
    src/tools/toolfactory.h
    src/tools/baseconvertertool.cpp
    src/tools/baseconvertertool.h
    src/tools/storageconvertertool.cpp
    src/tools/storageconvertertool.h
    src/tools/subnetcalculatortool.cpp
    src/tools/subnetcalculatortool.h
)

# Copy styles directory to build directory
file(COPY styles DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)
