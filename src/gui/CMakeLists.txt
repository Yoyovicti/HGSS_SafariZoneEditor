target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main_window.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/qselectitem.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui
)

add_subdirectory(main)
add_subdirectory(menu)
