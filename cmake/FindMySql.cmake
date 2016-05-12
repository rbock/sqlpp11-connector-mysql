# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIRS - where to find mysql.h, etc.
#  MYSQL_LIBRARIES    - List of libraries when using MySQL.
#  MYSQL_FOUND        - True if MySQL found.

if (NOT DEFINED MSVC)
	find_path(MYSQL_INCLUDE_DIR
		NAMES mysql.h
		PATH_SUFFIXES mysql
		)

	find_library(MYSQL_LIBRARY
		NAMES mysqlclient mysqlclient_r
		PATH_SUFFIXES mysql
		)
else()
	message("ENV::ProgramFiles: " "$ENV{ProgramFiles}")
	file(GLOB MYSQL_GLOB_PROGRAM "$ENV{ProgramFiles}/MySQL/" "include")
	message("MYSQL_GLOB_PROGRAM: " ${MYSQL_GLOB_PROGRAM})
	message("ENV::SystemDrive: " "$ENV{SystemDrive}")
	file(GLOB MYSQL_GLOB_SYSTEM "$ENV{SystemDrive}/MySQL/" "include")
	message("MYSQL_GLOB_SYSTEM: " ${MYSQL_GLOB_SYSTEM})
	message("ENV::ProgramFiles(x86): " "$ENV{ProgramFiles(x86)}")
	find_path(MYSQL_INCLUDE_DIR
		NAMES mysql.h
		PATHS ${MYSQL_GLOB_PROGRAM}
					${MYSQL_GLOB_SYSTEM}
		)

	find_library(MYSQL_LIBRARY
		NAMES mysqlclient mysqlclient_r
		PATHS "$ENV{PROGRAMFILES}/MySQL/*/lib"
					"$ENV{SYSTEMDRIVE}/MySQL/*/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYSQL
  FOUND_VAR MYSQL_FOUND
  REQUIRED_VARS MYSQL_LIBRARY MYSQL_INCLUDE_DIR
  )

mark_as_advanced(
  MYSQL_LIBRARY
  MYSQL_INCLUDE_DIR
  )

set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})
