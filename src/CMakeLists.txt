add_library(sqlpp-mysql
		connection.cpp
		prepared_statement.cpp
		char_result.cpp
		bind_result.cpp
		detail/connection_handle.cpp)

if (HAS_LIBCPP_VERSION)
	target_link_libraries(sqlpp-mysql "${Boost_THREAD_LIBRARY}")
endif()

install(TARGETS sqlpp-mysql DESTINATION lib)
