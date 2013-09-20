
#pragma once
#ifndef __SLCORE_H__
#define __SLCORE_H__

//!---------------------------------------------------------------------------
//! \file "slCore.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


// TODO: root slDebug.h
#include "platform/win32/slDebug.h"

// Windows Header Files
// note: regarding this include
//       other sources might want to include windows too
//       but be want to customize the big windows include
//       so we include windows early
#include "platform/win32/slWindows.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h> // move to top ?
#include <memory.h> // move to top ?
#include <tchar.h>

#include <stdio.h>
#include <math.h>

#include "platform/win32/slWindowsTypes.h"
#include "universal/core/slCoreTypes.h"
#include "universal/core/slSTL.h"
#include "universal/core/slMultithreading.h"
#include "universal/math/slMathMacros.h"
#include "universal/math/slRandom.h"
#include "universal/animation/slAnimation.h"


#include "slInstanceCount.h"

#include "sltl/slFlagSet.h"
#include "sltl/slMathTemplates.h"
#include "sltl/slCircularVector.h"


//-----------------------------------------------------------------------------

#endif  // __SLCORE_H__
