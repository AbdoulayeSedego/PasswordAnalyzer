add_executable(gui
        main.cpp          # Explicitly include—must be first for entry point
        mainwindow.cpp
        mainwindow.ui     # Qt auto-processes .ui for moc/uic
)

target_link_libraries(gui
        PRIVATE Qt6::Core
        PRIVATE Qt6::Widgets
        PRIVATE common     # Your shared lib
)

# macOS bundle for app-like executable (no console)
set_target_properties(gui PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourname.PasswordAnalyzerGUI"
)

# Optional: Suppress console on run (for clean GUI)
if(APPLE)
    set_target_properties(gui PROPERTIES
            OUTPUT_NAME "PasswordAnalyzerGUI"
    )
endif()