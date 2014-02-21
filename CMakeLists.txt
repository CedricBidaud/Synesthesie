cmake_minimum_required(VERSION 2.8)

project(Imac3-Synesthésie)

# System libraries
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)

# GLM: math library
include_directories(third-party/glm-0.9.4.6)

# GLEW: to use OpenGL3+ functions
include_directories(third-party/glew-1.10.0/include)
add_definitions(-DGLEW_STATIC=1)
add_subdirectory(third-party/glew-1.10.0)

# IMGUI :
include_directories(third-party/my_imgui)

#FMOD
include_directories(/usr/include/FMOD)
# set the path to the library folder
link_directories(/usr/lib)

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Our application
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)


add_executable(Synesthésie ${SRC_FILES})
target_link_libraries(Synesthésie glew ${OPENGL_LIBRARIES} ${SDL_LIBRARY} libfmodex64.so)

