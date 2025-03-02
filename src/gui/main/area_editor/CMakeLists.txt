target_sources(${PROJECT_NAME} PRIVATE
    # Area view
    ${CMAKE_CURRENT_SOURCE_DIR}/widget_3dview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object_item.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_editor/object_view.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_editor/object_item.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_editor
)
