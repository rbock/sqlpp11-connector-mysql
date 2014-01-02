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


#ifndef SQLPP_MYSQL_PREPARED_QUERY_H
#define SQLPP_MYSQL_PREPARED_QUERY_H

#include <memory>
#include <sqlpp11/integral.h>

namespace sqlpp
{
	namespace mysql
	{
		struct connection;

		namespace detail
		{
			struct prepared_query_handle_t;
		}

		class prepared_query_t
		{
			friend ::sqlpp::mysql::connection;
			std::unique_ptr<detail::prepared_query_handle_t> _handle;

		public:
			prepared_query_t() = delete;
			prepared_query_t(std::unique_ptr<detail::prepared_query_handle_t>&& handle);
			prepared_query_t(const prepared_query_t&) = delete;
			prepared_query_t(prepared_query_t&& rhs);
			prepared_query_t& operator=(const prepared_query_t&) = delete;
			prepared_query_t& operator=(prepared_query_t&&);
			~prepared_query_t();

			bool operator==(const prepared_query_t& rhs) const
			{
				return _handle == rhs._handle;
			}

			void bind_boolean_parameter(size_t index, const bool* value, bool is_null);
			void bind_boolean_result(size_t index, bool* value, bool* is_null);

			void bind_integral_parameter(size_t index, const int64_t* value, bool is_null);
			void bind_integral_result(size_t index, int64_t* value, bool* is_null);

			void bind_text_parameter(size_t index, const char* value, size_t len);
			void bind_text_result(size_t index, char** text, size_t* len);
		};
	}
}
#endif
