############################################################################################################ 
# AutoExtern.cmake 
############################################################################################################

# Extern library to parse arguments
include(CMakeParseArguments)

############################################################################################################ 
# auto_download_archive
# 
# This function downloads and extracts the archive from the specified URL to the 
# given directory. Upon exit, it defines the ARCHIVE_<NAME>_DIR where the base dir
# of the extracted archive is located.
#
# ARCHIVE_NAME : The name of the archive
#  ARCHIVE_URL : The URL where to download from
#  ARCHIVE_EXT : The extension of the archive (ex. ".tar.gz") 
#  ARCHIVE_DIR : The directory where to extract into
#
# auto_download_archive(
#			NAME 		archiveName
#			URL 		archiveURL
#		   [EXTENSION	archiveExt]
# 			VAR_NAME 	varName
#		   [SHA512		checksum]
#		   [SHA256		checksum]
#		   [SHA1		checksum]
#		   [MD5			checksum]
#		   [DIRECTORY	baseDir]
# 		)
#
# Exposes the following information to the environment:
#
#  <VAR_NAME>_READY   : This is set to 1 when the file is downloaded and the checksum is validated
#  <VAR_NAME>_DIR     : The base dir of the expanded archive
#  <VAR_NAME>_ARCHIVE : The archive filename before extract
#
function( auto_download_archive )

	# Parse arguments
	set(options "")
	set(oneValueArgs NAME VAR_NAME URL SHA1 SHA256 SHA512 MD5 DIRECTORY EXTENSION)
	set(multiValueArgs "")
	cmake_parse_arguments(OPT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	# Use NAME if VAR_NAME is not specified
	if ( "${OPT_VAR_NAME}" STREQUAL "")
		set( OPT_VAR_NAME ${OPT_NAME} )
	endif()

	# Prepare variable names
	set( VAR_READY "${OPT_VAR_NAME}_READY" )
	set( VAR_DIR "${OPT_VAR_NAME}_DIR" )
	set( VAR_ARCHIVE "${OPT_VAR_NAME}_ARCHIVE" )

	# By default set ready to FALSE
	set( ${VAR_READY} 0 PARENT_SCOPE)

	# Calculate the archive dir
	if ( "${OPT_DIRECTORY}" STREQUAL "")
		set( ARCHIVE_DIR "${CMAKE_BINARY_DIR}/downloaded/${OPT_NAME}" )
	else()
		set( ARCHIVE_DIR "${OPT_DIRECTORY}" )
	endif()

	# Try to auto-detect extension
	if ( "${OPT_EXTENSION}" STREQUAL "")
		string(REGEX REPLACE "^.*(\\.\\w+)(\\?.*|$)" "\\1" OPT_EXTENSION ${OPT_URL})

		# Check if we did not find an exteion (starting with '.'')
		string(SUBSTRING ${OPT_EXTENSION} 0 1 CHECK_EXT)
		if ( NOT "${CHECK_EXT}" STREQUAL "." )

			# Perform heuristics to get some clues regarding the extension
			string(REGEX MATCH "zip" MATCH_ZIP ${OPT_URL})
			if ( NOT "${MATCH_ZIP}" STREQUAL "" )
				set(OPT_EXTENSION ".zip")
			else()
				string(REGEX MATCH "bz2?" MATCH_BZ ${OPT_URL})
				if ( NOT "${MATCH_BZ}" STREQUAL "" )
					set(OPT_EXTENSION ".tar.bz2")
				else()
					set(OPT_EXTENSION ".tar.gz")
				endif()
			endif()
		endif()

	endif()

	# Lookup archive name
	set( ARCHIVE_FILE "${ARCHIVE_DIR}${OPT_EXTENSION}" )

	# Check if we have the archive directory
	if ( NOT EXISTS ${ARCHIVE_DIR} )

		# Prepare 'EXPECTED_HASH'
		set(EXPECTED_HASH "")
		if ( NOT "${OPT_SHA512}" STREQUAL "")
			set(EXPECTED_HASH "SHA512=${OPT_SHA512}")
		elseif( NOT "${OPT_SHA256}" STREQUAL "")
			set(EXPECTED_HASH "SHA256=${OPT_SHA256}")
		elseif( NOT "${OPT_SHA1}" STREQUAL "")
			set(EXPECTED_HASH "SHA1=${OPT_SHA1}")
		elseif( NOT "${OPT_MD5}" STREQUAL "")
			set(EXPECTED_HASH "MD5=${OPT_MD5}")
		endif()

		# Download the archive
		message( STATUS "Downloading ${OPT_NAME} archive" )
		if ( "${EXPECTED_HASH}" STREQUAL "")
			# Don't use checksum verification & enable TLS verify
			# as a precaution of downloading the correct file.
			file(DOWNLOAD 
				${OPT_URL} 
				${ARCHIVE_FILE}
				SHOW_PROGRESS 
				STATUS ARCHIVE_DOWNLOAD_STATUS
				TLS_VERIFY on
			)
		else()
			# Use checksum for verification & disable TLS verify.
			file(DOWNLOAD 
				${OPT_URL} 
				${ARCHIVE_FILE}
				SHOW_PROGRESS 
				STATUS ARCHIVE_DOWNLOAD_STATUS
				EXPECTED_HASH ${EXPECTED_HASH}
				TLS_VERIFY off
			)
		endif()

		# Check status
		list( GET ARCHIVE_DOWNLOAD_STATUS 0 ARCHIVE_DOWLOAD_ERROR )
		if( ARCHIVE_DOWLOAD_ERROR )

			# Remove file
			file( REMOVE ${ARCHIVE_FILE} )

			# Fatal error
			message( FATAL_ERROR "Could not download ${OPT_NAME} archive!" )
		endif()

		# Make the directory
		FILE( MAKE_DIRECTORY ${ARCHIVE_DIR} )

		# Extract archive
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xzf ${ARCHIVE_FILE}
			WORKING_DIRECTORY ${ARCHIVE_DIR}
		)

	endif()

	# Get the first sub-directory in the archive
	file(GLOB _ARCHIVE_SUBDIRS ${ARCHIVE_DIR}/*)
	foreach( _ARCHIVE_SUBDIR ${_ARCHIVE_SUBDIRS} )
		if(IS_DIRECTORY ${_ARCHIVE_SUBDIR})
			set(ARCHIVE_DIR ${_ARCHIVE_SUBDIR})
		endif()
	endforeach()

	# Expose the actual base folder of the project
	set( ${VAR_READY} 1 PARENT_SCOPE )
	set( ${VAR_DIR} ${ARCHIVE_DIR} PARENT_SCOPE )
	set( ${VAR_ARCHIVE} ${ARCHIVE_FILE} PARENT_SCOPE )
	message( STATUS "Archive ${OPT_NAME} downloaded" )

endfunction()


############################################################################################################ 
# auto_include_library
#
# auto_include_library(
#		NAME        libraryName
#	   [VAR_NAME	variableName]
#	    PACKAGE	 	findPackageName
#      [VERSION     packageVersion]
#      [COMPONENTS  findPackage Components]
# 		LOCAL_DIR	localCMakeFolder
#	   [LOCAL_ONLY]
#	)
#
function( auto_include_library )

	# Parse arguments
	set(options LOCAL_ONLY)
	set(oneValueArgs NAME VAR_NAME PACKAGE VERSION LOCAL_DIR)
	set(multiValueArgs COMPONENTS)
	cmake_parse_arguments(OPT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	# Use NAME if VAR_NAME is not specified
	if ( "${OPT_VAR_NAME}" STREQUAL "")
		set( OPT_VAR_NAME ${OPT_NAME} )
	endif()

	# Prepare variable names
	set( VAR_LIBRARIES "${OPT_VAR_NAME}_LIBRARIES" )
	set( VAR_INCLUDE_DIRS "${OPT_VAR_NAME}_INCLUDE_DIRS" )
	set( VAR_SYSTEM_FLAG "${OPT_VAR_NAME}_FROM_SYSTEM" )
	set( VAR_MODE "${OPT_VAR_NAME}_MODE" )
	set( VAR_COMPONENTS "${OPT_VAR_NAME}_COMPONENTS" )
	set( VAR_VERSION "${OPT_VAR_NAME}_VERSION" )

	# Check some missing flags
	if (NOT DEFINED ${${VAR_SYSTEM_FLAG}})
		set(${VAR_SYSTEM_FLAG} OFF)
	endif()

	# Start library checkup
	set( ${VAR_MODE} "EXTERN" PARENT_SCOPE )
	if ( NOT DEFINED ${VAR_LIBRARIES} OR NOT DEFINED ${VAR_INCLUDE_DIRS} )

		# Check if we should check for local
		set( USE_LOCAL 1 )
		if ( (NOT ${OPT_LOCAL_ONLY} OR ${${VAR_SYSTEM_FLAG}}) AND NOT ("${OPT_PACKAGE}" STREQUAL "") )

			# Check if we can find it on the system
			if ( "${OPT_COMPONENTS}" STREQUAL "")
				if ("${OPT_LOCAL_DIR}" STREQUAL "")
					find_package(${OPT_PACKAGE} ${OPT_VERSION})
				else()
					find_package(${OPT_PACKAGE} ${OPT_VERSION} QUIET)
				endif()
			else()
				if ("${OPT_LOCAL_DIR}" STREQUAL "")
					find_package(${OPT_PACKAGE} ${OPT_VERSION} COMPONENTS ${OPT_COMPONENTS})
				else()
					find_package(${OPT_PACKAGE} ${OPT_VERSION} QUIET COMPONENTS ${OPT_COMPONENTS})
				endif()
			endif()

			# Check different flavors of FOUND
			if (NOT DEFINED ${OPT_PACKAGE}_FOUND )
				set( ${OPT_PACKAGE}_FOUND FALSE )
			endif()
			if (NOT DEFINED ${OPT_VAR_NAME}_FOUND )
				set( ${OPT_VAR_NAME}_FOUND FALSE )
			endif()

			# Check if we had the _found flag
			if( ${${OPT_PACKAGE}_FOUND} OR ${${OPT_VAR_NAME}_FOUND} )

				# We are using system's, skip local
				set( USE_LOCAL 0 )

				# Pick libraries & includes from package
				set( ${VAR_MODE} "SYSTEM" PARENT_SCOPE )
				if ( ${${OPT_VAR_NAME}_FOUND} )
					set( ${VAR_LIBRARIES} ${${OPT_VAR_NAME}_LIBRARY} PARENT_SCOPE)
					set( ${VAR_LIBRARIES} ${${VAR_LIBRARIES}} ${${OPT_VAR_NAME}_LIBRARIES} PARENT_SCOPE )
					set( ${VAR_INCLUDE_DIRS} ${${OPT_VAR_NAME}_INCLUDE_DIR} PARENT_SCOPE )
					set( ${VAR_INCLUDE_DIRS} ${${VAR_INCLUDE_DIRS}} ${${OPT_VAR_NAME}_INCLUDE_DIRS} PARENT_SCOPE )
				else()
					set( ${VAR_LIBRARIES} ${${OPT_PACKAGE}_LIBRARY} PARENT_SCOPE )
					set( ${VAR_LIBRARIES} ${${VAR_LIBRARIES}} ${${OPT_PACKAGE}_LIBRARIES} PARENT_SCOPE )
					set( ${VAR_INCLUDE_DIRS} ${${OPT_PACKAGE}_INCLUDE_DIR} PARENT_SCOPE )
					set( ${VAR_INCLUDE_DIRS} ${${VAR_INCLUDE_DIRS}} ${${OPT_PACKAGE}_INCLUDE_DIRS} PARENT_SCOPE )
				endif()

				message( STATUS "Using ${OPT_NAME} from system")

			endif()
		endif()

		# Use shipped library if not found locally
		if(${USE_LOCAL})
			if ("${OPT_LOCAL_DIR}" STREQUAL "")

				# Could not build/find library!
				message( FATAL_ERROR "Could not find ${OPT_NAME} library!")

			else()

				# Prepare variables
				set( ${VAR_COMPONENTS} ${OPT_COMPONENTS} )
				set( ${VAR_VERSION} ${OPT_VERSION} )

				# Include library project		
				add_subdirectory( ${OPT_LOCAL_DIR} ${CMAKE_BINARY_DIR}/extern/lib${NAME} )

				# Pick libraries & includes from local store
				set( ${VAR_MODE} "LOCAL" PARENT_SCOPE )
				set( ${VAR_LIBRARIES} ${${VAR_LIBRARIES}} PARENT_SCOPE )
				set( ${VAR_INCLUDE_DIRS} ${${VAR_INCLUDE_DIRS}} PARENT_SCOPE )
				message( STATUS "Using ${OPT_NAME} from local folder")

			endif()

		endif()

	else()

		# We already have this library on scope
		message( STATUS "Using ${OPT_NAME} from: ${${VAR_INCLUDE_DIRS}}")

	endif()

endfunction()