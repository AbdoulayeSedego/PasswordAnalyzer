cmake_minimum_required(VERSION 3.16)
project(PasswordStrengthAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt for GUI (CLion auto-detects if in path)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)  # Or Qt5 if using older

# Subdirs: Builds CLI, GUI, common in order
add_subdirectory(common)
add_subdirectory(cli)
add_subdirectory(gui)

# Optional: Global install or tests later