#==============================================================================
# nfx-datatypes - CMake Sources
#==============================================================================

#----------------------------------------------
# Conditional headers and sources
#----------------------------------------------

set(PUBLIC_HEADERS)
set(PRIVATE_HEADERS)
set(PRIVATE_SOURCES)

list(APPEND PUBLIC_HEADERS
	# --- Datatype headers ---
	${NFX_DATATYPES_INCLUDE_DIR}/nfx/datatypes/Decimal.h
	${NFX_DATATYPES_INCLUDE_DIR}/nfx/datatypes/Int128.h

	# --- Datatype implementations ---
	${NFX_DATATYPES_INCLUDE_DIR}/nfx/detail/datatypes/Decimal.inl
	${NFX_DATATYPES_INCLUDE_DIR}/nfx/detail/datatypes/Int128.inl
)
list(APPEND PRIVATE_HEADERS
	# --- Datatype headers files ---
	${NFX_DATATYPES_SOURCE_DIR}/Constants.h
)
list(APPEND PRIVATE_SOURCES
	# --- Datatype source files ---
	${NFX_DATATYPES_SOURCE_DIR}/Decimal.cpp
	${NFX_DATATYPES_SOURCE_DIR}/Int128.cpp
)

#----------------------------------------------
# Library definition
#----------------------------------------------

# --- Create shared library if requested ---
if(NFX_DATATYPES_BUILD_SHARED)
	add_library(${PROJECT_NAME} SHARED)
	target_sources(${PROJECT_NAME}
		PRIVATE
			${PUBLIC_HEADERS}
			${PRIVATE_HEADERS}
			${PRIVATE_SOURCES}
	)

	set_target_properties(${PROJECT_NAME} PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
		ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	)

	add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

# --- Create static library if requested ---
if(NFX_DATATYPES_BUILD_STATIC)
	add_library(${PROJECT_NAME}-static STATIC)
	target_sources(${PROJECT_NAME}-static
		PRIVATE
			${PUBLIC_HEADERS}
			${PRIVATE_HEADERS}
			${PRIVATE_SOURCES}
	)

	set_target_properties(${PROJECT_NAME}-static PROPERTIES
		OUTPUT_NAME ${PROJECT_NAME}-static-${PROJECT_VERSION}
		ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	)

	add_library(${PROJECT_NAME}::static ALIAS ${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Target properties
#----------------------------------------------

function(configure_target target_name)
	# --- Include directories ---
	target_include_directories(${target_name}
		PUBLIC
			$<BUILD_INTERFACE:${NFX_DATATYPES_INCLUDE_DIR}>
			$<INSTALL_INTERFACE:include>
		PRIVATE
			${NFX_DATATYPES_SOURCE_DIR}
	)

	# --- Properties ---
	set_target_properties(${target_name} PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
		POSITION_INDEPENDENT_CODE ON
		DEBUG_POSTFIX "-d"
	)
endfunction()

# --- Apply configuration to both targets ---
if(NFX_DATATYPES_BUILD_SHARED)
	configure_target(${PROJECT_NAME})
	if(WIN32)
		set_target_properties(${PROJECT_NAME} PROPERTIES
			WINDOWS_EXPORT_ALL_SYMBOLS TRUE
		)

		configure_file(
			${CMAKE_CURRENT_SOURCE_DIR}/cmake/nfxDataTypesVersion.rc.in
			${CMAKE_BINARY_DIR}/nfxDataTypes.rc
			@ONLY
		)
		target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/nfxDataTypes.rc)
	endif()
endif()

if(NFX_DATATYPES_BUILD_STATIC)
	configure_target(${PROJECT_NAME}-static)
endif()
