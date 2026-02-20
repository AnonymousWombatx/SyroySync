cmake_minimum_required(VERSION 3.16)

project(SyroySync VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appSyroySync
    main.cpp
)

qt_add_qml_module(appSyroySync
    URI SyroySync
    QML_FILES
        Main.qml
        RESOURCES resources/images/intro_background.jpg
        QML_FILES intro.qml
        RESOURCES resources/images/intro_background.jpg resources/images/SyroySync.png
        RESOURCES resources/font/Quicksand.ttf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSyroySync PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSyroySync
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSyroySync
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appSyroySync
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
