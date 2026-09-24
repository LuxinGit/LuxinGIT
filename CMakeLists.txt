cmake_minimum_required(VERSION 3.24)

project(LuxinPaint VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.22
    GIT_SHALLOW TRUE
)

set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.2b
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
)

add_executable(LuxinPaint
    Application/Application.cpp
    Application/main.cpp
    Application/STRUCTS.cpp

    Canvas/Canvas.cpp
    Canvas/Cursor.cpp
    Canvas/Draw.cpp
    Canvas/Object.cpp

    Command/Action.cpp
    "Command/Command Repo.cpp"
    Command/Command.cpp
    Command/Input.cpp

    Platform/SDLWrapper.cpp
)

target_include_directories(LuxinPaint PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LuxinPaint PRIVATE
    SDL3::SDL3
    imgui
)

if(MSVC)
    target_compile_options(LuxinPaint PRIVATE /W3 /permissive-)
else()
    target_compile_options(LuxinPaint PRIVATE -Wall -Wextra)
endif()