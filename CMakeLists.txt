cmake_minimum_required(VERSION 3.5)

project(hgss_safari_zone_editor
    VERSION 0.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json REQUIRED)

add_compile_definitions(
    VERSION_MAJOR=${hgss_safari_zone_editor_VERSION_MAJOR}
    VERSION_MINOR=${hgss_safari_zone_editor_VERSION_MINOR}
)

set(PROJECT_SOURCES
    src/main.cpp

    # GUI
    src/gui/main_window.cpp
    # Top bar elements
    src/gui/file_menu.cpp
    src/gui/options_menu.cpp
    src/gui/lang_menu.cpp
    src/gui/about_message_box.cpp
    include/gui/file_menu.hpp
    include/gui/options_menu.hpp
    include/gui/lang_menu.hpp
    # Main elements
    src/gui/area_view.cpp
    src/gui/day_counters.cpp
    src/gui/hover_label.cpp
    src/gui/block_popup.cpp
    include/gui/area_view.hpp
    include/gui/hover_label.hpp
)

include_directories(
    include/
)

if(WIN32)
    set(RESOURCE_FILES assets/resource.rc)
else()
    set(RESOURCE_FILES)
endif()

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${RESOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Properties for MacOS are auto-generated and not tested
if(Qt6 VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.yoyovicti.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE ON
    WIN32_EXECUTABLE ON
)

file(
    COPY data assets
    DESTINATION .
)