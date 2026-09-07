cmake_minimum_required(VERSION 3.5)

project(PasswordManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.h
        src/MainWindow.cpp
        src/LoginDialog.h
        src/LoginDialog.cpp
        src/AddEditDialog.h
        src/AddEditDialog.cpp
        src/CryptoUtils.h
        src/CryptoUtils.cpp
        src/StorageManager.h
        src/StorageManager.cpp
        src/Entry.h
)

# Embed version info + app manifest on Windows so the exe shows
# proper publisher/description and execution level instead of blank
# metadata (blank metadata increases SmartScreen/SAC suspicion).
# NOTE: built as a separate OBJECT library with NO Qt linkage, because
# windres splits include paths containing spaces (e.g. "D:/QT Creator/...")
# when invoking its preprocessor. This target only needs src/ itself.
if(WIN32)
    add_library(app_metadata OBJECT src/version.rc)
    target_include_directories(app_metadata PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    list(APPEND PROJECT_SOURCES $<TARGET_OBJECTS:app_metadata>)
endif()

add_executable(PasswordManager
    ${PROJECT_SOURCES}
)

target_link_libraries(PasswordManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
