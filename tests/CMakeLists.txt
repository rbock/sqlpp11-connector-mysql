macro (build_and_run arg)
	add_executable(${arg} ${arg}.cpp)
	target_link_libraries (${arg} sqlpp-mysql)
	target_link_libraries (${arg} mysqlclient)
	add_test(${arg} ${arg})
endmacro ()

build_and_run(SampleTest)
build_and_run(SelectTest)
