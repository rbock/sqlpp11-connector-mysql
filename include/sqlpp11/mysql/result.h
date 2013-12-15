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


#ifndef SQLPP_MYSQL_RESULT_H
#define SQLPP_MYSQL_RESULT_H

#include <memory>
#include <sqlpp11/raw_result_row.h>

namespace sqlpp
{
	namespace mysql
	{
		namespace detail
		{
			struct result_handle;
			class result_impl_t
			{
				std::unique_ptr<result_handle> _handle;

			public:
				result_impl_t();
				result_impl_t(std::unique_ptr<detail::result_handle>&& handle);
				result_impl_t(const result_impl_t&) = delete;
				result_impl_t(result_impl_t&& rhs);
				result_impl_t& operator=(const result_impl_t&) = delete;
				result_impl_t& operator=(result_impl_t&&);
				~result_impl_t();

				bool operator==(const result_impl_t& rhs) const
				{
					return _handle == rhs._handle;
				}

				raw_result_row_t next();
				size_t num_cols() const;
			};
		}

		template<typename ResultRow, typename DynamicNames>
		struct result
		{
			using result_row_t = ResultRow;
			using dynamic_names_t = DynamicNames;
			detail::result_impl_t _impl;
			result_row_t _result_row;

			result()
			{}

			result(detail::result_impl_t&& impl):
				_impl(std::move(impl))
			{}

			result(const result&) = delete;
			result(result&& rhs) = default;
			result& operator=(const result&) = delete;
			result& operator=(result&&) = default;
			~result() = default;

			bool operator==(const result& rhs) const
			{
				return _impl == rhs._impl;
			}

			const result_row_t& next()
			{
				_result_row = _impl.next();
				return _result_row;
			};

			size_t num_cols() const;
		};

	}
}
#endif
