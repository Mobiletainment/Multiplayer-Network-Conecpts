#pragma once
#ifndef __SLCONFIG_H__
#define __SLCONFIG_H__

//!---------------------------------------------------------------------------
//! \file slConfig.h
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------



#ifdef WIN32
#  include "platform/win32/slDebug.h"
#  include "platform/win32/slWindows.h"
#else
#  include "TargetConditionals.h"
#endif
 

#endif // __SLCONFIG_H__
