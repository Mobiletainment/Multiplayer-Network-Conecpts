//!---------------------------------------------------------------------------
//! \file "slSTL.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------

#include "slConfig.h"
#include "slSTL.h"

namespace sl	{
	//------------------------------------------------------------------------------
	SLAString& replaceString(SLAString& subject, const SLAString& search,
		const SLAString& replace) {
			size_t pos = 0;
			while((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
	}
}

