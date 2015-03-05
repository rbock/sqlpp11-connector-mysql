macro (build_and_run arg)
	# Add headers to sources to enable file browsing in IDEs
	add_executable(Sqlpp11MySQL${arg} ${arg}.cpp ${sqlpp_headers})
	target_link_libraries(Sqlpp11MySQL${arg} sqlpp-mysql)
	target_link_libraries(Sqlpp11MySQL${arg} mysqlclient)
	add_test(${arg} Sqlpp11MySQL${arg})
endmacro ()

build_and_run(SampleTest)
build_and_run(SelectTest)
build_and_run(UnionTest)
build_and_run(DynamicSelectTest)
