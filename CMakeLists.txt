cmake_minimum_required(VERSION 3.16)

project(SyroySync VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/src/app.rc")

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appSyroySync
    main.cpp
    ${app_icon_resource_windows}
)

qt_add_qml_module(appSyroySync
    URI SyroySync
    VERSION 1.0

    QML_FILES
        qml/Main.qml
        qml/IntroPage.qml
        qml/LoadPage.qml
        qml/inputOverview.qml
        qml/APIPage.qml
        qml/BrowserPage.qml
        qml/WebsitePage.qml
        qml/SettingsPage.qml
        QML_FILES qml/DownloadDrawer.qml
        QML_FILES qml/Information.qml

    SOURCES
        src/updater.h          src/updater.cpp
        src/youtubeservice.h   src/youtubeservice.cpp
        src/videomodel.h       src/videomodel.cpp
        src/downloadservice.h  src/downloadservice.cpp
        src/basedownloader.h
        src/ytdlpplugin.h src/ytdlpplugin.cpp
        SOURCES src/downloadjob.h src/downloadjob.cpp
        SOURCES src/structures.h
        SOURCES src/downloadmanager.h src/downloadmanager.cpp

    RESOURCES
        qml/resources/images/API.png
        qml/resources/images/API_background.jpg
        qml/resources/images/Browser.png
        qml/resources/images/input_overview_background.jpg
        qml/resources/images/intro_background.jpg
        qml/resources/images/loading_background.jpg
        qml/resources/images/male_lion.jpg
        qml/resources/images/next_arrow.png
        qml/resources/images/settings_background.jpg
        qml/resources/images/SyroySync.png
        qml/resources/images/Website.png
        qml/resources/font/Quicksand.ttf
        qml/resources/data/extensions.json
        qml/resources/images/folder.png
        qml/resources/images/info_background.jpg
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
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
)

target_compile_definitions(appSyroySync PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
    APP_GITHUB="https://github.com/AnonymousWombatx/SyroySync/releases/latest"
)

include(GNUInstallDirs)
install(TARGETS appSyroySync
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
