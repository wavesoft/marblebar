cmake_minimum_required (VERSION 2.8)

# MarbleBar Library
project ( marblebar )
set(VERSION "0.0.1")

# Add properties
option(SYSTEM_MONGOOSE "Set to ON to use libMongoose from the system" OFF)
option(SYSTEM_JSONCPP "Set to ON to use jsoncpp from the system" OFF)

# Include additional libraries
include(cmake/AddCompileLinkFlags.cmake)

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
	message( STATUS "Using Mongoose shipped with libcernvm")

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
	message( STATUS "Using JsonCPP shipped with libcernvm")

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
	)

# Setup includes
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/src )
include_directories( ${PROJECT_INCLUDES} )

#############################################################
# TARGET & LINK
#############################################################

# Add library target
add_library( 
	${PROJECT_NAME}
	${PROJECT_SOURCES}
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

