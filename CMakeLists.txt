cmake_minimum_required (VERSION 2.8)

# Guess system architecture
set(GUESS_ARCH "i386") 							# 1) Fallback to x86
if(CMAKE_SYSTEM_PROCESSOR) 						# 2) Build on the same processor
	set(GUESS_ARCH ${CMAKE_SYSTEM_PROCESSOR})
	if(${GUESS_ARCH} STREQUAL "AMD64")
		# We don't do any differentiation for AMD64 instruction set
		set(GUESS_ARCH "x86_64")
	endif()
endif()
if(CMAKE_OSX_ARCHITECTURES)						# 3) Lookup on OSX Architectures
	set(GUESS_ARCH ${CMAKE_OSX_ARCHITECTURES})
elseif ("${CMAKE_GENERATOR}" MATCHES "Win64") 	# 4) Lookup on Windows Generator
	set(GUESS_ARCH "x86_64")
endif()

# Prompt architecture
set(TARGET_ARCH "${GUESS_ARCH}" CACHE STRING "Override the identified target architecture (x86_64 or i386)" )

# MarbleBar Library
project ( marblebar )
set(VERSION "0.0.1")

# Add properties
option(SYSTEM_MONGOOSE "Set to ON to use libMongoose from the system" OFF)
option(SYSTEM_JSONCPP "Set to ON to use jsoncpp from the system" OFF)

# Include additional libraries
include(cmake/AddCompileLinkFlags.cmake)

# Banner
message(STATUS "Building ${PROJECT_NAME} for arch: ${TARGET_ARCH}")

#############################################################
# LIBRARIES 
#############################################################

# For every library, we give the option to the project that
# includes us to provide them by itself.
#
# If not, we will try our best to build them as our child
# libraries (statically).
#
# To define a library, you must define the following two directives:
#   XXXXXX_LIBRARIES : Which contain the name(s) of the libraries
#	XXXXXX_INCLUDE 	 : Which contain the additiona include directories
#

#
# [Mongoose] For the Web Server
#
set( MONGOOSE_MODE "EXTERN" )
if ( NOT DEFINED(MONGOOSE_LIBRARIES) OR NOT DEFINED(MONGOOSE_INCLUDE_DIRS) )

	# We are shipping mongoose with the project
	set( EXTERN_MONGOOSE "extern/mongoose" )
	add_subdirectory( ${EXTERN_MONGOOSE} ${CMAKE_BINARY_DIR}/${EXTERN_MONGOOSE} )

	# Log
	set( MONGOOSE_MODE "LOCAL" )
	message( STATUS "Using Mongoose shipped with library")

else()
	message( STATUS "Using Mongoose from: ${MONGOOSE_INCLUDE_DIRS}")

endif()

#
# [JSONCPP] For parsing JSON data
#
set( JSONCPP_MODE "EXTERN" )
if ( NOT DEFINED(JSONCPP_LIBRARIES) OR NOT DEFINED(JSONCPP_INCLUDE_DIRS) )

	# We are shipping jsoncpp with the project
	set( EXTERN_JSONCPP "extern/jsoncpp" )
	add_subdirectory( ${EXTERN_JSONCPP} ${CMAKE_BINARY_DIR}/${EXTERN_JSONCPP} )

	# Log
	set( JSONCPP_MODE "LOCAL" )
	message( STATUS "Using JsonCPP shipped with library")

else()
	message( STATUS "Using JsonCPP from: ${JSONCPP_INCLUDE_DIRS}")

endif()

# Include libraries
set( PROJECT_INCLUDES
	${MONGOOSE_INCLUDE_DIRS}
	${JSONCPP_INCLUDE_DIRS}
)

# Collect library names
set( PROJECT_LIBRARIES 
	${MONGOOSE_LIBRARIES}
	${JSONCPP_LIBRARIES}
)

#############################################################
# GENERATED SOURCES
#############################################################

# Lookup perl, we need it 
find_package(Perl)
if(NOT PERL_FOUND)
    message( FATAL_ERROR "MarbleBar requires perl for building. On windows try ActivePerl (http://www.activestate.com/activeperl)" )
endif()

# Pick a filename where to place the embedded file contents
set( GEN_RESOURCES_C "${PROJECT_BINARY_DIR}/generated_data.cpp" )

# Collect web resources
file ( GLOB_RECURSE HTDOCS_RESOURCES
		RELATIVE ${PROJECT_SOURCE_DIR}/htdocs
		# Get HTML Websites
		${PROJECT_SOURCE_DIR}/htdocs/*
	)

# Collect resources and build executable
execute_process(
	COMMAND ${PERL_EXECUTABLE} "${PROJECT_SOURCE_DIR}/src/mkdata.pl" ${HTDOCS_RESOURCES}
	OUTPUT_FILE ${GEN_RESOURCES_C}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/htdocs
	)

#############################################################
# SOURCES 
#############################################################

# Locate the daemon sources
file ( GLOB PROJECT_SOURCES 
		${PROJECT_SOURCE_DIR}/src/*.cpp
		${PROJECT_SOURCE_DIR}/src/properties/*.cpp
	)


# Locate platform-dependant sources
if (WIN32)
	set ( PLATFORM_DIR ${PROJECT_SOURCE_DIR}/src/platform/win )
	file ( GLOB PLATFORM_SOURCES ${PLATFORM_DIR}/*.cpp ${PLATFORM_DIR}/*.rc )
elseif (APPLE)
	set ( PLATFORM_DIR ${PROJECT_SOURCE_DIR}/src/platform/osx )
	file ( GLOB PLATFORM_SOURCES ${PLATFORM_DIR}/*.mm  )
elseif(UNIX)
	set ( PLATFORM_DIR ${PROJECT_SOURCE_DIR}/src/platform/unix )
	file ( GLOB PLATFORM_SOURCES ${PLATFORM_DIR}/*.cpp  )
endif()

# Setup includes
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/src )
include_directories( ${PROJECT_INCLUDES} )

#############################################################
# TARGET & LINK
#############################################################

# Platform dependencies
if (APPLE)
	find_library(FRAMEWORK_COCOA NAMES Cocoa)
endif()

# Add library target
add_library( 
	${PROJECT_NAME}
	${PROJECT_SOURCES}
	${PLATFORM_SOURCES}
	${GEN_RESOURCES_C}
)

# Enable C++11 extensions
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	add_compile_flags( ${PROJECT_NAME} -std=c++11 )
elseif(COMPILER_SUPPORTS_CXX0X)
	add_compile_flags( ${PROJECT_NAME} -std=c++0x )
else()
        message(CRITICAL "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Project libraries
target_link_libraries ( ${PROJECT_NAME} ${PROJECT_LIBRARIES} )

# Add apple frameworks
if (APPLE)
	target_link_libraries ( ${PROJECT_NAME} ${FRAMEWORK_COCOA} )
endif()

# Expose useful information in the parent scope
set( MarbleBar_LIBS ${PROJECT_NAME} PARENT_SCOPE)
set( MarbleBar_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${PROJECT_INCLUDES} PARENT_SCOPE)

