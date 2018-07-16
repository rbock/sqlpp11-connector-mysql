sqlpp11-connector-mysql
=======================

A C++ wrapper for mysql meant to be used in combination with sqlpp11 (https://github.com/rbock/sqlpp11).

Status:
-------
Branch / Compiler | clang-3.4,  gcc-4.9, Xcode-7   |  MSVC 2015
------------------| -------------------------------|-----------
master | [![Build Status](https://travis-ci.org/rbock/sqlpp11-connector-mysql.svg?branch=master)](https://travis-ci.org/rbock/sqlpp11-connector-mysql?branch=master) | [![Build status](https://ci.appveyor.com/api/projects/status/8yo0sbopv63kb2t7/branch/master?svg=true)](https://ci.appveyor.com/project/rbock/sqlpp11-connector-mysql/branch/master)
develop | [![Build Status](https://travis-ci.org/rbock/sqlpp11-connector-mysql.svg?branch=develop)](https://travis-ci.org/rbock/sqlpp11-connector-mysql?branch=develop) | [![Build status](https://ci.appveyor.com/api/projects/status/8yo0sbopv63kb2t7/branch/develop?svg=true)](https://ci.appveyor.com/project/rbock/sqlpp11-connector-mysql/branch/develop) |

Remark regarding MSVC builds on appveyor: As of now, I can neither to link against the provided mysql-connector-c library nor build it from scratch. I am currently compiling the mariadb client and link against that one. Build is fine, tests fail. Help is appreciated.


License Hint:
-------------
The code is distributed under BSD License, but if you build the library, then the binary will be linked dynamically to mysqlclient, which is published under GPL or commercial license as of this writing. The resulting binary might therefore fall under GPL. To avoid this, an option to link against the equivalent MariaDB connector (under LGPL) is provided: see the build instructions below.

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
	for(const auto& row : db.run(sqlpp::select(all_of(tab)).from(tab).unconditionally()))
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

  * clang-3.4 on Ubuntu-12.4 (requires thread_local)
  * g++-4.8 on Ubuntu-12.4
  * MSVC 2015 Update 2 on appveyor

__C++ SQL Layer:__
sqlpp11-connector-mysql is meant to be used with sqlpp11 (https://github.com/rbock/sqlpp11).

__mysqlclient:__
libmysqlclient version 5.5 and up (or the respective mariadb version)



Build instructions:
-------------------
Download and unpack the latest release or clone the repository. Inside the directory run the following commands:

```bash
mkdir build
cd build
cmake ..
make
make install
```

In order to customize the build process, you can add the following options to the cmake command above:

* CMAKE_INSTALL_PREFIX: path to the install folder
* DATE_INCLUDE_DIR: path to the folder containing `date.h`
* SQLPP11_INCLUDE_DIR: path to the folder containing `sqlpp11.h`
* USE_MARIADB: force the use of the MariaDB connector instead of the GPL'd MySQL connector
 
For example:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/opt/sqlpp11_mariadb -DUSE_MARIADB=1 ..
```
