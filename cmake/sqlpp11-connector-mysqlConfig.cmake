list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

include(CMakeFindDependencyMacro)
find_dependency(date REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/sqlpp11-connector-mysqlTargets.cmake")
