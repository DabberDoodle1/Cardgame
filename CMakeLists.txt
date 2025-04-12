cmake_minimum_required(VERSION 3.27)
project(cardgame VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if (NOT WIN32)
    string(ASCII 27 ESC)
    set(BLUE "${ESC}[1;33m")
endif()

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/stb_image)

set(
    CARDGAME_SOURCES
    "${CMAKE_SOURCE_DIR}/src/cardgame.cpp"
    "${CMAKE_SOURCE_DIR}/src/cards.cpp"
    "${CMAKE_SOURCE_DIR}/src/game.cpp"
    "${CMAKE_SOURCE_DIR}/src/resource_manager.cpp"
    "${CMAKE_SOURCE_DIR}/src/shader.cpp"
    "${CMAKE_SOURCE_DIR}/src/sprite_renderer.cpp"
    "${CMAKE_SOURCE_DIR}/src/texture.cpp"
)

add_custom_target(
    copy_res ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res"
    "${CMAKE_BINARY_DIR}/res"
    COMMENT "${BLUE}Resource files have been successfully copied into binary directory"
)

add_executable("${CMAKE_PROJECT_NAME}")
set_property(
    TARGET "${CMAKE_PROJECT_NAME}"
    PROPERTY CXX_STANDARD 20
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1)
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${CARDGAME_SOURCES})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries("${CMAKE_PROJECT_NAME}" glad glfw glm stb_image)
