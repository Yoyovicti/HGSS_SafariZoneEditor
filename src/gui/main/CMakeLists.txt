target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/safari_layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/day_counters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/area_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/area_selector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/area_item.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/safari_layout.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/day_counters.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/day_counter_edit.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_view.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_selector.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_item.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui/main
)

add_subdirectory(area_editor)
add_subdirectory(safari_layout)
