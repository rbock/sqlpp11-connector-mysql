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
	find_path(MYSQL_INCLUDE_DIR
		NAMES mysql.h
		PATH_SUFFIXES include
		PATHS "$ENV{SystemDrive}/Program Files/mariadb-connector-c/include/mariadb"
					"$ENV{SystemDrive}/Program Files (x86)/mariadb-connector-c/include/mariadb"
		)
	MESSAGE("INCLUDE_DIR: ${MYSQL_INCLUDE_DIR}")

	find_library(MYSQL_LIBRARY
		NAMES libmariadb
		PATH_SUFFIXES lib
		PATHS "$ENV{SystemDrive}/Program Files/mariadb-connector-c/lib/mariadb"
					"$ENV{SystemDrive}/Program Files (x86)/mariadb-connector-c/lib/mariadb"
		)
	MESSAGE("LIB: ${MYSQL_LIBRARY}")
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
