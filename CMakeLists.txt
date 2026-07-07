cmake_minimum_required(VERSION 3.16)
project(mindev LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(GetGitRevision)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg)

set(APP_SOURCES
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

if(WIN32)
    list(APPEND APP_SOURCES "${CMAKE_SOURCE_DIR}/resources.rc")
endif()

add_executable(${PROJECT_NAME} ${APP_SOURCES})

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
endif()

# Copy styles directory to build directory
file(COPY styles DESTINATION ${CMAKE_BINARY_DIR})

# Copy icons to build directory
file(COPY icons DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg)

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${APP_VERSION}")

# Convert SVG to PNG after build
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND cmd /c "rsvg-convert -w 64 -h 64 \"${CMAKE_BINARY_DIR}\\icons\\app.svg\" -o \"${CMAKE_BINARY_DIR}\\icons\\app.png\""
        COMMENT "Converting SVG icon to PNG"
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND bash -c "rsvg-convert -w 64 -h 64 '${CMAKE_BINARY_DIR}/icons/app.svg' -o '${CMAKE_BINARY_DIR}/icons/app.png'"
        COMMENT "Converting SVG icon to PNG"
    )
endif()
