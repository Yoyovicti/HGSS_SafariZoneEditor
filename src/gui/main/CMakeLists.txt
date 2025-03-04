target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/safari_layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/day_counters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/area_view.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/safari_layout.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/day_counters.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/day_counter_edit.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_view.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/main/area_selector.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui/main
)

add_subdirectory(area_editor)
add_subdirectory(safari_layout)
