
add_library(sqlpp-mysql )
add_library(sqlpp11::mysql ALIAS sqlpp-mysql)

target_sources(sqlpp-mysql
	PRIVATE
	connection.cpp
	prepared_statement.cpp
	char_result.cpp
	bind_result.cpp
	detail/connection_handle.cpp)

target_link_libraries(sqlpp-mysql
	PUBLIC sqlpp11::sqlpp11
	MySQL::MySQL
)

target_include_directories(sqlpp-mysql INTERFACE
	"$<BUILD_INTERFACE:${sqlpp11-connector-mysql_SOURCE_DIR}/include>"
	$<INSTALL_INTERFACE:include>
)

install(TARGETS sqlpp-mysql DESTINATION lib)
