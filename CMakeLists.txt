cmake_minimum_required(VERSION 3.5)

project(hgss_safari_zone_editor
    VERSION 0.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(nlohmann_json REQUIRED)
find_package(assimp 5 REQUIRED)

message(STATUS "assimp_FOUND: ${assimp_FOUND}")
message(STATUS "assimp_DIR: ${assimp_DIR}")

add_compile_definitions(
    VERSION_MAJOR=${hgss_safari_zone_editor_VERSION_MAJOR}
    VERSION_MINOR=${hgss_safari_zone_editor_VERSION_MINOR}
)


message(ASSIMP_INCLUDE_DIRS="${ASSIMP_INCLUDE_DIRS}")
message(ASSIMP_LIBRARIES="${ASSIMP_LIBRARIES}")

if(WIN32)
    set(RESOURCE_FILES ${CMAKE_SOURCE_DIR}/assets/resource.rc)
else()
    set(RESOURCE_FILES)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${RESOURCE_FILES}
)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    assimp::assimp
)

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
    COPY
        data
        assets
    DESTINATION .
)
