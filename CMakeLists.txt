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

add_executable(PasswordManager
    ${PROJECT_SOURCES}
)

target_link_libraries(PasswordManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
