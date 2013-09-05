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
#include <sqlpp11/mysql/result.h>
#include "detail/result_handle.h"


namespace sqlpp
{
	namespace mysql
	{
		result::result(std::unique_ptr<detail::result_handle>&& handle, const bool debug):
			_handle(std::move(handle)),
			_debug(debug)
		{
			if (_debug)
				std::cerr << "MySQL debug: Constructing result, using handle at " << _handle.get() << std::endl;
		}

		result::~result() = default;
		result::result(result&& rhs) = default;
		result& result::operator=(result&&) = default;

		raw_result_row_t result::next()
		{
			if (_debug)
				std::cerr << "MySQL debug: Accessing next row of handle at " << _handle.get() << std::endl;

			return _handle 
				? raw_result_row_t{ const_cast<const char**>(mysql_fetch_row(_handle->mysql_res)), mysql_fetch_lengths(_handle->mysql_res) }
				: raw_result_row_t{ nullptr, nullptr };
		}

		size_t result::num_cols() const
		{
			return _handle
				? mysql_num_fields(_handle->mysql_res)
				: 0;
		}

	}
}

