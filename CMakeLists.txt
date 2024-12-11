cmake_minimum_required(VERSION 3.19)
project(imp_drawing LANGUAGES CXX)

# Find Qt modules (add Network to the list)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

# Set up the project (Qt-specific configurations)
qt_standard_project_setup()

# Add executable and sources
qt_add_executable(imp_drawing
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    udpmanager.h udpmanager.cpp
    myscene.h myscene.cpp
    myview.h myview.cpp
    changerecord.h
)

# Link Qt libraries, including the Network module
target_link_libraries(imp_drawing
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

# Install rules (optional)
include(GNUInstallDirs)

install(TARGETS imp_drawing
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Deploy script (optional)
qt_generate_deploy_app_script(
    TARGET imp_drawing
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
