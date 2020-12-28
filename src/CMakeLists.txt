
add_library(sqlpp-mysql SHARED)
add_library(sqlpp11::mysql ALIAS sqlpp-mysql)

add_library(sqlpp-mysql-static STATIC)
add_library(sqlpp11::mysql::static ALIAS sqlpp-mysql-static)

set(sqlpp_SRCs
	connection.cpp
	prepared_statement.cpp
	char_result.cpp
	bind_result.cpp
	detail/connection_handle.cpp)

target_sources(sqlpp-mysql PRIVATE ${sqlpp_SRCs})
target_sources(sqlpp-mysql-static PRIVATE ${sqlpp_SRCs})

target_link_libraries(sqlpp-mysql
	PUBLIC sqlpp11::sqlpp11
	MySQL::MySQL
)

target_link_libraries(sqlpp-mysql-static
	PUBLIC sqlpp11::sqlpp11
	MySQL::MySQL
)

if (NOT MSVC)
	set_target_properties(sqlpp-mysql-static PROPERTIES OUTPUT_NAME sqlpp-mysql)
endif()

target_include_directories(sqlpp-mysql INTERFACE
	"$<BUILD_INTERFACE:${sqlpp11-connector-mysql_SOURCE_DIR}/include>"
	$<INSTALL_INTERFACE:include>
)

target_include_directories(sqlpp-mysql-static INTERFACE
	"$<BUILD_INTERFACE:${sqlpp11-connector-mysql_SOURCE_DIR}/include>"
	$<INSTALL_INTERFACE:include>
)

install(TARGETS sqlpp-mysql sqlpp-mysql-static DESTINATION lib)
