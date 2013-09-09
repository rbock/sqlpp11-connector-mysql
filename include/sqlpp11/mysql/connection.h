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


#ifndef SQLPP_MYSQL_CONNECTION_H
#define SQLPP_MYSQL_CONNECTION_H

#include <string>
#include <sqlpp11/connection.h>
#include <sqlpp11/mysql/result.h>
#include <sqlpp11/mysql/connection_config.h>
#include <sqlpp11/detail/tag.h>

namespace sqlpp
{
	namespace mysql
	{
		namespace detail
		{
			class connection_handle;
		}

		class connection: public sqlpp::connection
		{
			std::unique_ptr<detail::connection_handle> _handle;
			bool _transaction_active = false;

		public:
			using _result_t = ::sqlpp::mysql::result;
			struct _tags
			{
				using _has_empty_list_insert = tag_yes;
			};
			connection(const std::shared_ptr<connection_config>& config);
			~connection();
			connection(const connection&) = delete;
			connection(connection&&) = delete;
			connection& operator=(const connection&) = delete;
			connection& operator=(connection&&) = delete;

			//! select returns a result (which can be iterated row by row)
			_result_t select(const std::string& query);

			//! insert returns the last auto_incremented id (or zero, if there is none)
			size_t insert(const std::string& query);

			//! update returns the number of affected rows
			size_t update(const std::string& query);

			//! remove returns the number of removed rows
			size_t remove(const std::string& query);

			//! execute arbitrary command (e.g. create a table)
			void execute(const std::string& command);

			//! escape given string (does not quote, though)
			std::string escape(const std::string& s) const;

			//! call run on the argument
			template<typename T>
				auto run(const T& t) -> decltype(t.run(*this))
				{
					return t.run(*this);
				}

			//! start transaction
			void start_transaction();

			//! commit transaction (or throw transaction if the transaction has been finished already)
			void commit_transaction();

			//! rollback transaction with or without reporting the rollback (or throw if the transaction has been finished already)
			void rollback_transaction(bool report);

			//! report a rollback failure (will be called by transactions in case of a rollback failure in the destructor)
			void report_rollback_failure(const std::string message) noexcept;
		};

	}
}
#endif
