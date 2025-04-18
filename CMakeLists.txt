cmake_minimum_required(VERSION 3.10)
project(GameOfLife VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set raylib path
set(RAYLIB_PATH "C:/raylib/raylib" CACHE PATH "Path to raylib source directory")

# Configure static linking
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)


# Add source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/game.h
    src/grid.cpp
    src/grid.h
    src/globals.cpp
    src/globals.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Add raylib as a subdirectory
add_subdirectory(${RAYLIB_PATH} ${CMAKE_BINARY_DIR}/raylib)

# Link with Raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Copy font files to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Font DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
) 