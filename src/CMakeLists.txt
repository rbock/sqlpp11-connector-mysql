add_library(sqlpp-mysql 
		connection.cpp
		prepared_query.cpp
		result.cpp
		detail/connection_handle.cpp)

install(TARGETS sqlpp-mysql DESTINATION lib)
