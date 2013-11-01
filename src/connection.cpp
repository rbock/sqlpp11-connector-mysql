/*
 * Copyright (c) 2013, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <iostream>
#include <sqlpp11/exception.h>
#include <sqlpp11/mysql/connection.h>
#include "detail/result_handle.h"
#include "detail/connection_handle.h"

namespace sqlpp
{
	namespace mysql
	{
		namespace
		{
			struct MySqlThreadInitializer
			{
				MySqlThreadInitializer()
				{
					mysql_thread_init();
				}
				~MySqlThreadInitializer()
				{
					mysql_thread_end();
				}
			};

			void execute_query(detail::connection_handle& handle, const std::string& query)
			{
				thread_local MySqlThreadInitializer threadInitializer;

				if (handle.config->debug)
					std::cerr << "MySQL debug: Executing: '" << query << "'" << std::endl;

				if (mysql_query(handle.mysql.get(), query.c_str()))
        {
					throw sqlpp::exception("MySQL error: Could not execute MySQL-query: " + std::string(mysql_error(handle.mysql.get())) + " (query was >>" + query + "<<\n");
        }
			}
		}

		connection::connection(const std::shared_ptr<connection_config>& config):
			_handle(new detail::connection_handle(config))
		{
			if (mysql_set_character_set(_handle->mysql.get(), _handle->config->charset.c_str()))
			{
				throw sqlpp::exception("MySQL error: can't set character set " + _handle->config->charset);
			}

			if (mysql_select_db(_handle->mysql.get(), _handle->config->database.c_str()))
			{
				throw sqlpp::exception("MySQL error: can't select database '" + _handle->config->database + "'");
			}
		}

		connection::~connection()
		{
		}

		connection::_result_t connection::select(const std::string& query)
		{
			execute_query(*_handle, query);
			std::unique_ptr<detail::result_handle> result_handle(new detail::result_handle(mysql_store_result(_handle->mysql.get())));
			if (!result_handle)
			{
				throw sqlpp::exception("MySQL error: Could not store result set: " + std::string(mysql_error(_handle->mysql.get())));
			}

			return result(std::move(result_handle), _handle->config->debug);
		}

		size_t connection::insert(const std::string& query)
		{
			execute_query(*_handle, query);

			return mysql_insert_id(_handle->mysql.get());
		}

		void connection::execute(const std::string& command)
		{
			execute_query(*_handle, command);
		}

		size_t connection::update(const std::string& query)
		{
			execute_query(*_handle, query);
			return mysql_affected_rows(_handle->mysql.get());
		}

		size_t connection::remove(const std::string& query)
		{
			execute_query(*_handle, query);
			return mysql_affected_rows(_handle->mysql.get());
		}

		std::string connection::escape(const std::string& s) const
		{
			char dest[s.size() * 2 + 1];
			mysql_real_escape_string(_handle->mysql.get(), dest, s.c_str(), s.size());
			return dest;
		}

		void connection::start_transaction()
		{
			if (_transaction_active)
			{
				throw sqlpp::exception("Cannot have more than one open transaction per connection");
			}
			execute_query(*_handle, "START TRANSACTION");
			_transaction_active = true;
		}

		void connection::commit_transaction()
		{
			if (not _transaction_active)
			{
				throw sqlpp::exception("Cannot commit a finished or failed transaction");
			}
			_transaction_active = false;
			execute_query(*_handle, "COMMIT");
		}

		void connection::rollback_transaction(bool report)
		{
			if (not _transaction_active)
			{
				throw sqlpp::exception("Cannot rollback a finished or failed transaction");
			}
			if (report)
			{
				std::cerr << "MySQL warning: Rolling back unfinished transaction" << std::endl;
			}
			_transaction_active = false;
			execute_query(*_handle, "ROLLBACK");
		}

		void connection::report_rollback_failure(const std::string message) noexcept
		{
			std::cerr << "MySQL message:" << message << std::endl;
		}
	}
}

