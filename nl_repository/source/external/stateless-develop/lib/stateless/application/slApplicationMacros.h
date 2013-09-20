//!---------------------------------------------------------------------------
//! \file "slApplicationMacros.h"
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
#ifndef __SLAPPLICATIONMACROS_H__
#define __SLAPPLICATIONMACROS_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2013, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

#define SL_POCO_PTR( className ) typedef Poco::AutoPtr<className> className##Ptr;
#define SL_PROCESS_APP() sl::ProcessApplication::sharedInstance()

#endif // __SLAPPLICATIONMACROS_H__
