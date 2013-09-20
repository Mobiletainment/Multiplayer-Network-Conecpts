//!---------------------------------------------------------------------------
//! \file "slApplicationEnums.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLAPPLICATIONENUMS_H__
#define __SLAPPLICATIONENUMS_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2013, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

namespace sl	{

	//-------------------------------------------------------------------------
	enum ELogType
	{
		ELogType_Info,
		ELogType_Status,
		ELogType_Message,
		ELogType_Warning,
		ELogType_Error,
		ELogType_Count,
	};

	//-------------------------------------------------------------------------
	enum EConsoleBackColor
	{
		EConsoleBackColor_Black,
		EConsoleBackColor_Grey,
	};

}

#endif // __SLAPPLICATIONENUMS_H__
