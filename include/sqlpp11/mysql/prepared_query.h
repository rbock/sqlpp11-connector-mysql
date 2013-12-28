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

namespace sqlpp
{
	namespace mysql
	{
		namespace detail
		{
			struct prepared_query_handle_t;
			class prepared_query_impl_t
			{
				std::unique_ptr<prepared_query_handle_t> _handle;

			public:
				prepared_query_impl_t();
				prepared_query_impl_t(std::unique_ptr<detail::prepared_query_handle_t>&& handle);
				prepared_query_impl_t(const prepared_query_impl_t&) = delete;
				prepared_query_impl_t(prepared_query_impl_t&& rhs);
				prepared_query_impl_t& operator=(const prepared_query_impl_t&) = delete;
				prepared_query_impl_t& operator=(prepared_query_impl_t&&);
				~prepared_query_impl_t();

				bool operator==(const prepared_query_impl_t& rhs) const
				{
					return _handle == rhs._handle;
				}
			};
		}

		template<typename ResultRow, typename DynamicNames>
		struct prepared_query_t
		{
			using result_row_t = ResultRow;
			using dynamic_names_t = DynamicNames;
			detail::prepared_query_impl_t _impl;
			dynamic_names_t _dynamic_names;

			prepared_query_t()
			{}

			prepared_query_t(detail::prepared_query_impl_t&& impl, const dynamic_names_t& dynamic_names):
				_impl(std::move(impl)),
				_dynamic_names(dynamic_names)
			{
			}

			prepared_query_t(const prepared_query_t&) = delete;
			prepared_query_t(prepared_query_t&& rhs) = default;
			prepared_query_t& operator=(const prepared_query_t&) = delete;
			prepared_query_t& operator=(prepared_query_t&&) = default;
			~prepared_query_t() = default;

			bool operator==(const prepared_query_t& rhs) const
			{
				return _impl == rhs._impl;
			}
		};

	}
}
#endif
