/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_STD_CONVERSION_FUNCTIONS_H_
#define RTTR_STD_CONVERSION_FUNCTIONS_H_

#include "rttr/detail/base/core_prerequisites.h"
#include <string>

namespace rttr
{
namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API bool char_to_bool(const char* source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API int char_to_int(const char* source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API long long char_to_long_long(const char* source, bool& ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API unsigned int char_to_uint(const char* source, bool& ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API float char_to_float(const char* source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API double char_to_double(const char* source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API std::string int_to_string(int value, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API std::string float_to_string(float value, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API std::string double_to_string(double value, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API bool string_to_bool(std::string value, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API int string_to_int(const std::string& source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API float string_to_float(const std::string& source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

RTTR_API double string_to_double(const std::string& source, bool* ok);

/////////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_STD_CONVERSION_FUNCTIONS_H_
