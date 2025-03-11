target_sources(${PROJECT_NAME} PRIVATE
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_subdirectory(gui)
