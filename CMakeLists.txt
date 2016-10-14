cmake_minimum_required(VERSION 3.1)
project( CG )

#SET(CMAKE_BUILD_TYPE "None")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(Freetype REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_definitions(-DLOG_COLOR)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DLOG_FINE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_definitions(-DLOG_SEVERE)
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DLOG_INFO)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

include_directories(include/)
aux_source_directory(src/ SOURCE_FILES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
