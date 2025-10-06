add_executable(gui
        main.cpp
        mainwindow.cpp
        mainwindow.ui  # Auto-moc for .ui
)
target_link_libraries(gui
        Qt6::Core
        Qt6::Widgets
        common  # Shared analyzer
)
set_target_properties(gui PROPERTIES
        WIN32_EXECUTABLE TRUE  # No console on Win
        MACOSX_BUNDLE TRUE
)