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
#include <sqlpp11/mysql/bind_result.h>
#include "detail/prepared_query_handle.h"


namespace sqlpp
{
	namespace mysql
	{
		bind_result_t::bind_result_t(const std::shared_ptr<detail::prepared_query_handle_t>& handle):
			_handle(handle)
		{
			if (_handle and _handle->debug)
				std::cerr << "MySQL debug: Constructing bind result, using handle at " << _handle.get() << std::endl;
		}

		void bind_result_t::bind_integral_result(size_t index, int64_t* value, bool* is_null)
		{
			if (_handle->debug)
				std::cerr << "binding integral result " << *value << " at index: " << index << std::endl;

			detail::result_meta_data_t& meta_data = _handle->result_param_meta_data[index];
			meta_data.len = nullptr;
			meta_data.is_null = is_null;

			MYSQL_BIND& param = _handle->result_params[index];
			param.buffer_type = MYSQL_TYPE_LONGLONG;
			param.buffer = value;
			param.buffer_length = sizeof(*value);
			param.length = &meta_data.bound_len;
			param.is_null = &meta_data.bound_is_null;
			param.is_unsigned = false;
			param.error = &meta_data.bound_error;
		}

		void bind_result_t::bind_impl()
		{
			if (_handle and _handle->debug)
				std::cerr << "MySQL debug: Accessing next row of handle at " << _handle.get() << std::endl;

			auto flag = mysql_stmt_bind_result(_handle->mysql_stmt, _handle->result_params.data());
		}

		bool bind_result_t::next_impl()
		{
			if (_handle and _handle->debug)
				std::cerr << "MySQL debug: Accessing next row of handle at " << _handle.get() << std::endl;

			auto flag = mysql_stmt_fetch(_handle->mysql_stmt);

			switch(flag)
			{
			case 0:
				std::cerr << "address: " << _handle->result_params[0].buffer << std::endl;
				std::cerr << "value: " << *reinterpret_cast<int64_t*>(_handle->result_params[0].buffer) << std::endl;
				return true;
			case 1:
				throw sqlpp::exception(std::string("Could not fetch next result: ") + mysql_stmt_error(_handle->mysql_stmt));
			case MYSQL_NO_DATA:
				return false;
			case MYSQL_DATA_TRUNCATED:
				throw sqlpp::exception("Data truncation not handled yet");
			default:
				throw sqlpp::exception("Unexpected return value for mysql_stmt_fetch()");
			}
		}
	}
}

