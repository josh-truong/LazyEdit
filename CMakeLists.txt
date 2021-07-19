cmake_minimum_required(VERSION 3.16.3)
project(LazyEdit C CXX)
set(CMAKE_CXX_STANDARD 14)


# TURN OFF UNNECESSARY ACTIONS
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# Download SUBMODULES
find_package(GIT QUIET)
if(!GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# UPDATE SUBMODULES
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()


# CHECK ALL SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "Submodule glfw is required for this build!")
endif()


# Add subdirectories
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/FFmpeg)



list(APPEND HEADER_FILES
    "include/Shader.h"
    "include/LazyEdit.h"
    "include/Callbacks.h"
)
list(APPEND SOURCES
    "src/Shader.cpp"
    "src/LazyEdit.cpp"
    "src/Renderer.cpp"
    "src/OutputRawFile.cpp"
)
list(APPEND APP
    app/main.cpp
    # app/demuxing_decoding.cpp
)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE WIN32 ${APP} ${HEADER_FILES} ${SOURCES})

# OS specific
if(UNIX)
    list(APPEND EXTRA_LIBS
    "-lGL -lGLU -lX11"
    )
else ()
    message(FATAL_ERROR "THIS IS A UNIX BUILD")
endif()


# Include dirs
target_include_directories(${PROJECT_NAME}
    PUBLIC include/
    PUBLIC lib/glfw/include/
    PUBLIC external/glm
)


# Source dirs
target_include_directories(${PROJECT_NAME}
    PRIVATE src/
    PRIVATE lib/glfw/src/
)


# Link libs
target_link_libraries(
    ${PROJECT_NAME}
    ${EXTRA_LIBS}
    glfw
    FFmpeg
)