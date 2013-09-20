//!---------------------------------------------------------------------------
//! \file "slMultiThreading.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slConfig.h"

namespace sl	{

	void threadSleep( size_t milliseconds )
	{
#ifdef WIN32
		::Sleep( static_cast<DWORD>(milliseconds) );
#else
		::usleep(1000 * milliseconds);
#endif
	}

}

