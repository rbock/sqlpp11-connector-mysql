#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

#include <iostream>

namespace mysql = sqlpp::mysql;

int main()
{
  mysql::global_library_init();

  auto config = std::make_shared<mysql::connection_config>();
  config->user = "root";
  config->database = "sqlpp_mysql";
  config->debug = true;
  try
  {
    mysql::connection db(config);
    std::cerr << "OK" << std::endl;
  }
  catch (const sqlpp::exception& e)
  {
    std::cerr << "For testing, you'll need to create a database sqlpp_mysql for user root (no password)" << std::endl;
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
