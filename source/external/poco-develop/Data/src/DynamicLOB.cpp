//
// DynamicLOB.cpp
//
// $Id: //poco/Main/Data/src/DynamicLOB.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  DynamicLOB
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifdef __GNUC__
// TODO: determine g++ version able to do the right thing without these specializations

#include "Poco/Data/DynamicLOB.h"
#include "Poco/Data/LOB.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace Dynamic {


using Poco::Data::CLOB;
using Poco::Data::BLOB;


template <>
Var::operator CLOB () const
{
	VarHolder* pHolder = content();

	if (!pHolder)
		throw InvalidAccessException("Can not convert empty value.");

	if (typeid(CLOB) == pHolder->type())
		return extract<CLOB>();
	else
	{
		std::string result;
		pHolder->convert(result);
		return CLOB(result);
	}
}


template <>
Var::operator BLOB () const
{
	VarHolder* pHolder = content();

	if (!pHolder)
		throw InvalidAccessException("Can not convert empty value.");

	if (typeid(BLOB) == pHolder->type())
		return extract<BLOB>();
	else
	{
		std::string result;
		pHolder->convert(result);
		return BLOB(reinterpret_cast<const unsigned char*>(result.data()),
			result.size());
	}
}


} } // namespace Poco::Data


#endif // __GNUC__

