# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIRS - where to find mysql.h, etc.
#  MYSQL_LIBRARIES    - List of libraries when using MySQL.
#  MYSQL_FOUND        - True if MySQL found.

if (NOT DEFINED MSVC)
	if (USE_MARIADB)
		find_path(MYSQL_INCLUDE_DIR
			NAMES mysql.h
			PATH_SUFFIXES mariadb
			)

		find_library(MYSQL_LIBRARY
			NAMES mariadb
			PATH_SUFFIXES mariadb
			)
	else()
		find_path(MYSQL_INCLUDE_DIR
			NAMES mysql.h
			PATH_SUFFIXES mysql
			)

		find_library(MYSQL_LIBRARY
			NAMES mysqlclient mysqlclient_r
			PATH_SUFFIXES mysql
			)
	endif()
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

mark_as_advanced(
  MYSQL_LIBRARY
  MYSQL_INCLUDE_DIR
  )

set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})
if(NOT TARGET MySQL::MySQL)
	add_library(MySQL::MySQL UNKNOWN IMPORTED)
	set_target_properties(MySQL::MySQL PROPERTIES
		IMPORTED_LOCATION				"${MYSQL_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES	"${MYSQL_INCLUDE_DIRS}")
endif()
