cmake_minimum_required(VERSION 3.5)

project(hgss_safari_zone_editor
    VERSION 0.2
    LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json REQUIRED)

add_compile_definitions(
    VERSION_MAJOR=${hgss_safari_zone_editor_VERSION_MAJOR}
    VERSION_MINOR=${hgss_safari_zone_editor_VERSION_MINOR}
)

set(PROJECT_SOURCES
    src/main.cpp
    # Qt widgets
    src/main_window.cpp
    src/file_menu.cpp
    src/lang_menu.cpp
    src/options_menu.cpp
    src/about_message_box.cpp
    src/day_counters.cpp
    include/main_window.hpp
    include/file_menu.hpp
    include/lang_menu.hpp
    include/options_menu.hpp
    include/about_message_box.hpp
    include/day_counters.hpp
)

include_directories(
    include/
)

if(WIN32)
    set(RESOURCE_FILES assets/resource.rc)
else()
    set(RESOURCE_FILES)
endif()

set(ASSET_FILES
    assets/safari_icons/Desert.png
    assets/safari_icons/Forest.png
    assets/safari_icons/Marshland.png
    assets/safari_icons/Meadow.png
    assets/safari_icons/Mountain.png
    assets/safari_icons/Peak.png
    assets/safari_icons/Plains.png
    "assets/safari_icons/Rocky Beach.png"
    assets/safari_icons/Savannah.png
    assets/safari_icons/Swamp.png
    assets/safari_icons/Wasteland.png
    assets/safari_icons/Wetland.png
)

set(DATA_FILES
    data/locale.json
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/locale_manager.hpp
        include/config_manager.hpp
        include/save_file_manager.hpp
        ${RESOURCE_FILES}
        ${ASSET_FILES}
        ${DATA_FILES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${RESOURCE_FILES}
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.yoyovicti.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(
    COPY data
    DESTINATION .
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
