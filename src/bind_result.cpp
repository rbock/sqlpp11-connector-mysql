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

		void bind_result_t::bind_boolean_result(size_t index, signed char* value, bool* is_null)
		{
			if (_handle->debug)
				std::cerr << "binding boolean result " << *value << " at index: " << index << std::endl;

			detail::result_meta_data_t& meta_data = _handle->result_param_meta_data[index];
			meta_data.index = index;
			meta_data.len = nullptr;
			meta_data.is_null = is_null;

			MYSQL_BIND& param = _handle->result_params[index];
			param.buffer_type = MYSQL_TYPE_TINY;
			param.buffer = value;
			param.buffer_length = sizeof(*value);
			param.length = &meta_data.bound_len;
			param.is_null = &meta_data.bound_is_null;
			param.is_unsigned = false;
			param.error = &meta_data.bound_error;
		}

		void bind_result_t::bind_integral_result(size_t index, int64_t* value, bool* is_null)
		{
			if (_handle->debug)
				std::cerr << "binding integral result " << *value << " at index: " << index << std::endl;

			detail::result_meta_data_t& meta_data = _handle->result_param_meta_data[index];
			meta_data.index = index;
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

		void bind_result_t::bind_text_result(size_t index, char** value, size_t* len)
		{
			if (_handle->debug)
				std::cerr << "binding text result at index: " << index << std::endl;

			detail::result_meta_data_t& meta_data = _handle->result_param_meta_data[index];
			meta_data.index = index;
			meta_data.len = len;
			meta_data.is_null = nullptr;
			meta_data.text_buffer = value;

			MYSQL_BIND& param = _handle->result_params[index];
			param.buffer_type = MYSQL_TYPE_STRING;
			param.buffer = meta_data.bound_text_buffer.data();
			param.buffer_length = meta_data.bound_text_buffer.size();
			param.length = &meta_data.bound_len;
			param.is_null = &meta_data.bound_is_null;
			param.is_unsigned = false;
			param.error = &meta_data.bound_error;
		}

		void bind_result_t::bind_impl()
		{
			if (_handle->debug)
				std::cerr << "MySQL debug: Binding results for handle at " << _handle.get() << std::endl;

			auto flag = mysql_stmt_bind_result(_handle->mysql_stmt, _handle->result_params.data());
		}

		bool bind_result_t::next_impl()
		{
			if (_handle->debug)
				std::cerr << "MySQL debug: Accessing next row of handle at " << _handle.get() << std::endl;

			auto flag = mysql_stmt_fetch(_handle->mysql_stmt);

			switch(flag)
			{
			case 0:
			case MYSQL_DATA_TRUNCATED:
				{
					bool need_to_rebind = false;
					for (auto& r:  _handle->result_param_meta_data)
					{
						if (r.len)
						{
							if (r.bound_is_null)
							{
								*r.text_buffer = nullptr;
								*r.len = 0;
							}
							else
							{
								if (r.bound_len > r.bound_text_buffer.size())
								{
									if (_handle->debug)
										std::cerr << "MySQL debug: Need to reallocate buffer at index " << r.index << " for handle at " << _handle.get() << std::endl;
									need_to_rebind = true;
									r.bound_text_buffer.resize(r.bound_len);
									MYSQL_BIND& param = _handle->result_params[r.index];
									param.buffer = r.bound_text_buffer.data();
									param.buffer_length = r.bound_text_buffer.size();

									if (mysql_stmt_fetch_column(_handle->mysql_stmt, _handle->result_params.data() + r.index, r.index, 0))
										throw sqlpp::exception(std::string("Fetch column after reallocate failed: ") + mysql_stmt_error(_handle->mysql_stmt));
								}
								*r.text_buffer = r.bound_text_buffer.data();
								*r.len = r.bound_len;
							}
						}
						if (r.is_null)
							*r.is_null = r.bound_is_null;
					}
					if (need_to_rebind)
						bind_impl();
				}
				return true;
			case 1:
				throw sqlpp::exception(std::string("Could not fetch next result: ") + mysql_stmt_error(_handle->mysql_stmt));
			case MYSQL_NO_DATA:
				return false;
			default:
				throw sqlpp::exception("Unexpected return value for mysql_stmt_fetch()");
			}
		}
	}
}

