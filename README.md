sqlpp11-connector-mysql
=======================

A C++ wrapper for mysql meant to be used in combination with sqlpp11 (https://github.com/rbock/sqlpp11).

License Hint:
-------------
The code is distributed under BSD License, but if you build the library, then the binary will be linked dynamically to mysqlclient, which is published under GPL or commercial license as of this writing. The resulting binary might therefore fall under GPL.

Sample Code:
------------
See for instance test/SampleTest.cpp

```C++
#include "TabSample.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

namespace mysql = sqlpp::mysql;
int main()
{
	auto config = std::make_shared<mysql::connection_config>();
 	config->user = "root";
 	config->database = "sqlpp_mysql";
	config->debug = true;
	mysql::connection db(config);

	TabSample tab;
	for(const auto& row: sqlpp::select(all_of(tab)).from(tab).run(db))
	{
		std::cerr << "row.alpha: " << row.alpha << ", row.beta: " << row.beta << ", row.gamma: " << row.gamma <<  std::endl;
	};
  return 0;
}
```


Requirements:
-------------
__Compiler:__
sqlpp11-connector-mysql makes use of C++11 and requires a recent compiler and STL. The following compilers are known to compile the test programs:

  * clang-3.2 on Ubuntu-12.4
  * g++-4.8 on Ubuntu-12.4

__Database Connector:__
sqlpp11-connector-mysql is meant to be used with sqlpp11 (https://github.com/rbock/sqlpp11).

