cmake_minimum_required(VERSION 3.16)
project(PasswordStrengthAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt auto-processing for .ui files
set(CMAKE_AUTOMOC ON)  # Meta-object compiler for slots
set(CMAKE_AUTOUIC ON)  # UI compiler for .ui to .h
set(CMAKE_AUTORCC ON)  # Resource compiler if needed

# Your Qt path
set(CMAKE_PREFIX_PATH "/Users/abdoulayesedego/Qt/6.9.2/macos")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_subdirectory(common)
add_subdirectory(cli)
add_subdirectory(gui)