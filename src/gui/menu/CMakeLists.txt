target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/file_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lang_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/about_message_box.cpp
    ${CMAKE_SOURCE_DIR}/include/gui/menu/file_menu.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/menu/options_menu.hpp
    ${CMAKE_SOURCE_DIR}/include/gui/menu/lang_menu.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/gui/menu
)
