# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIRS - where to find mysql.h, etc.
#  MYSQL_LIBRARIES    - List of libraries when using MySQL.
#  MYSQL_FOUND        - True if MySQL found.

find_path(MYSQL_INCLUDE_DIR mysql.h
  PATH_SUFFIXES mysql
  )

find_library(MYSQL_LIBRARY
  NAMES mysqlclient_r mysqlclient
  PATH_SUFFIXES mysql
  )

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
