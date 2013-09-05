add_library(sqlpp-mysql 
		connection.cpp
		result.cpp
		detail/connection_handle.cpp)

install(TARGETS sqlpp-mysql DESTINATION lib)
