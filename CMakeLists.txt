cmake_minimum_required(VERSION 3.20)

project(GLRenderer)

add_compile_definitions(SOURCE_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
# add_compile_definitions(DEBUG)

include_directories(src)

file(GLOB_RECURSE INC_LIST src/*.hpp)
file(GLOB_RECURSE SRC_LIST src/*.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${INC_LIST})

find_package(assimp REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(GLRenderer 
    ${SRC_LIST} 
    ${INC_LIST}
)

target_link_libraries(GLRenderer PRIVATE
    assimp::assimp
    glad::glad
    glfw
    glm::glm
)
