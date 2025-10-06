add_library(common STATIC  # Static lib for linking
        PasswordAnalyzer.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# No Qt here—pure C++