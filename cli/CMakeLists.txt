add_executable(cli main.cpp)
target_link_libraries(cli common)  # Link shared logic
# For dict: Code loads from ${CMAKE_SOURCE_DIR}/common_passwords.txt
