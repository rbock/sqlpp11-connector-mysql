add_library(sqlpp-mysql 
		connection.cpp
		prepared_query.cpp
		char_result.cpp
		bind_result.cpp
		detail/connection_handle.cpp)

install(TARGETS sqlpp-mysql DESTINATION lib)
