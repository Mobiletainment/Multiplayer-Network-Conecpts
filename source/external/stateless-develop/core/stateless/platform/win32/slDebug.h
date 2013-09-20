//!---------------------------------------------------------------------------
//! \file "slDebug.h"
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
#ifndef __SLDEBUG_H__
#define __SLDEBUG_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2005, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

#if defined(WIN32) || defined(_WIN32)
# ifdef _DEBUG

#  pragma message( "--------------------------" )
#  pragma message( " Windows Configuration: including crtdbg.h" )
#  pragma message( "--------------------------" )

#   include <crtdbg.h>
# endif
#endif


#endif //  __SLDEBUG_H__
