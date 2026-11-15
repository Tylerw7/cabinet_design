cmake_minimum_required(VERSION 3.25)

project(cabinetDesign
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

# stb_image
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glm)

# GLAD
add_library(glad STATIC
    external/glad/src/gl.c
)
target_include_directories(glad PUBLIC external/glad/include)

add_executable(cabinetDesign
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
)

target_link_libraries(cabinetDesign PRIVATE glad glfw glm::glm)
target_include_directories(cabinetDesign 
    PRIVATE 
        src
        include
        ${stb_SOURCE_DIR}
)