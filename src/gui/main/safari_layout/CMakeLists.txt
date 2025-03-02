target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/area_label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/block_popup.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/safari_layout/area_label.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui/main/safari_layout
)
