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


#ifndef SQLPP_MYSQL_BIND_RESULT_H
#define SQLPP_MYSQL_BIND_RESULT_H

#include <memory>

namespace sqlpp
{
	namespace mysql
	{
		namespace detail
		{
			struct prepared_query_handle_t;
		}

		class bind_result_t
		{
			std::shared_ptr<detail::prepared_query_handle_t> _handle;
			void* _result_row_address = nullptr;

		public:
			bind_result_t() = default;
			bind_result_t(const std::shared_ptr<detail::prepared_query_handle_t>& handle);
			bind_result_t(const bind_result_t&) = delete;
			bind_result_t(bind_result_t&& rhs) = default;
			bind_result_t& operator=(const bind_result_t&) = delete;
			bind_result_t& operator=(bind_result_t&&) = default;
			~bind_result_t() = default;

			bool operator==(const bind_result_t& rhs) const
			{
				return _handle == rhs._handle;
			}

			template<typename ResultRow>
			void next(ResultRow& result_row)
			{
				if (!_handle)
				{
					result_row.invalidate();
					return;
				}

				if (&result_row != _result_row_address)
				{
					result_row._bind(*this);
					bind_impl();
					_result_row_address = &result_row;
				}
				if (next_impl())
				{
					if (not result_row)
					{
						result_row.validate();
					}
				}
				else
				{
					if (result_row)
						result_row.invalidate();
				}
			};

			void bind_boolean_result(size_t index, signed char* value, bool* is_null);
			void bind_integral_result(size_t index, int64_t* value, bool* is_null);
			void bind_text_result(size_t index, char** text, size_t* len);

		private:
			void bind_impl();
			bool next_impl();
		};

	}
}
#endif
