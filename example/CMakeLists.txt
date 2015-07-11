cmake_minimum_required (VERSION 2.8)
project ( marblebar-example )

# Include MarbleBar
add_subdirectory( ".." marblebar )
include_directories( ${MarbleBar_INCLUDE_DIRS} )

# Compile Project (With C++11)
add_definitions(-std=c++11)
add_executable( ${PROJECT_NAME} example.cpp )

# Link MarbleBar
target_link_libraries( ${PROJECT_NAME} ${MarbleBar_LIBS} )
