add_library(sqlpp-mysql connection.cpp prepared_statement.cpp char_result.cpp
                        bind_result.cpp detail/connection_handle.cpp)

install(TARGETS sqlpp-mysql DESTINATION lib)
install(TARGETS sqlpp-mysql EXPORT sqlpp11-connector-mysqlTargets)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/sqlpp11-connector-mysqlConfigVersion.cmake"
  VERSION ${VERSION}
  COMPATIBILITY AnyNewerVersion)

export(
  EXPORT sqlpp11-connector-mysqlTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/sqlpp11-connector-mysqlTargets.cmake")

configure_file(
  ../cmake/sqlpp11-connector-mysqlConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/sqlpp11-connector-mysqlConfig.cmake"
  COPYONLY)

set(ConfigPackageLocation lib/cmake/sqlpp11-connector-mysql)
install(
  EXPORT sqlpp11-connector-mysqlTargets
  NAMESPACE sqlpp11::
  DESTINATION ${ConfigPackageLocation})

install(
  FILES
    "../cmake/sqlpp11-connector-mysqlConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/sqlpp11-connector-mysqlConfigVersion.cmake"
  DESTINATION ${ConfigPackageLocation})
