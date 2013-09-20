//!---------------------------------------------------------------------------
//! \file "stdafx.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// slCore includes most commonly used stuff
#include <stateless/slCore.h>

#include <stateless/cocos2d/slCocosIncludes.h>
#include <stateless/application/slApplication.h>

#define NL_DLL
// note: this macro might already been defined inside the project settings
#ifndef slFramework_EXPORTS
#  define slFramework_EXPORTS
#endif
#include "slFrameworkAPI.h"
