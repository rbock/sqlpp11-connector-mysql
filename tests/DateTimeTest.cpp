/*
 * Copyright (c) 2013 - 2015, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "TabSample.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

#include <iostream>
#include <vector>
#include <cassert>

namespace
{
  const auto now = ::date::floor<::std::chrono::milliseconds>(std::chrono::system_clock::now());
  const auto today = ::date::floor<::sqlpp::chrono::days>(now);
  const auto yesterday = today - ::sqlpp::chrono::days{1};

  template <typename L, typename R>
  auto require_equal(int line, const L& l, const R& r) -> void
  {
    if (l != r)
    {
      std::cerr << line << ": ";
      serialize(::sqlpp::wrap_operand_t<L>{l}, std::cerr);
      std::cerr << " != ";
      serialize(::sqlpp::wrap_operand_t<R>{r}, std::cerr);
      throw std::runtime_error("Unexpected result");
    }
  }
}

namespace mysql = sqlpp::mysql;
int main()
{
	auto config = std::make_shared<mysql::connection_config>();
 	config->user = "root";
 	config->database = "sqlpp_mysql";
	config->debug = true;
	try
	{
		mysql::connection db(config);
	}
	catch(const sqlpp::exception& )
	{
		std::cerr << "For testing, you'll need to create a database sqlpp_mysql for user root (no password)" << std::endl;
		throw;
	}
	mysql::connection db(config);
	db.execute(R"(DROP TABLE IF EXISTS tab_date_time)");
	db.execute(R"(CREATE TABLE tab_date_time (
		col_day_point DATE,
			col_time_point DATETIME(3)
			))");

	TabDateTime tab;
  try
  {
    db(insert_into(tab).default_values());
    for (const auto& row : db(select(all_of(tab)).from(tab).where(true)))
    {
      require_equal(__LINE__, row.colDayPoint.is_null(), true);
      require_equal(__LINE__, row.colDayPoint.value(), ::sqlpp::chrono::day_point{});
      require_equal(__LINE__, row.colTimePoint.is_null(), true);
      require_equal(__LINE__, row.colTimePoint.value(), ::sqlpp::chrono::mus_point{});
    }

    db(update(tab).set(tab.colDayPoint = today, tab.colTimePoint = now).where(true));

    for (const auto& row : db(select(all_of(tab)).from(tab).where(true)))
    {
      require_equal(__LINE__, row.colDayPoint.value(), today);
      require_equal(__LINE__, row.colTimePoint.value(), now);
    }

    db(update(tab).set(tab.colDayPoint = yesterday, tab.colTimePoint = today).where(true));

    for (const auto& row : db(select(all_of(tab)).from(tab).where(true)))
    {
      require_equal(__LINE__, row.colDayPoint.value(), yesterday);
      require_equal(__LINE__, row.colTimePoint.value(), today);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

	return 0;
}
