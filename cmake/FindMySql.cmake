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
	file(GLOB MYSQL_GLOB_INCLUDE "$ENV{SystemDrive}/Program Files/mariadb-connector-c/lib/mariadb/include")
	file(GLOB MYSQL_GLOB_INCLUDE86 "$ENV{SystemDrive}/Program Files (x86)/mariadb-connector-c/lib/mariadb/include")
	find_path(MYSQL_INCLUDE_DIR
		NAMES mysql.h
		PATH_SUFFIXES include
		PATHS ${MYSQL_GLOB_INCLUDE}
					${MYSQL_GLOB_INCLUDE86}
		)

	file(GLOB MYSQL_GLOB_LIB "$ENV{SystemDrive}/Program Files/mariadb-connector-c/lib/mariadb/lib")
	file(GLOB MYSQL_GLOB_LIB86 "$ENV{SystemDrive}/Program Files (x86)/mariadb-connector-c/lib/mariadb/lib")
	find_library(MYSQL_LIBRARY
		NAMES mariadb-connector-c/lib/mariadb/libmariadb.dll
		PATH_SUFFIXES lib
		PATHS ${MYSQL_GLOB_LIB}
					${MYSQL_GLOB_LIB86}
		)
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
