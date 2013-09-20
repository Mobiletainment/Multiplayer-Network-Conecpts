//!---------------------------------------------------------------------------
//! \file "slMath.h"
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
#ifndef __SLMATH_H__
#define __SLMATH_H__

/*
 *  slMath.h
 *
 *  Created by Cyrus Preuss on 1/7/09.
 *  Copyright 2009 Cyrus Preuss. All rights reserved.
 *
 */

// optional stl include for C++
#ifndef SL_HAVE_STL
#  define SL_HAVE_STL 1
#endif

#include <math.h>
#include "../core/slMacros.h"
#include "slMathMacros.h"

#if SL_HAVE_STL
#  include "../core/slSTL.h"
#  define SLMin std::min
#  define SLMax std::max
#endif


static SL_INLINE SLFloat SLRadians( SLFloat x ) { return x * SL_RADS_PER_DEG; }
static SL_INLINE SLFloat SLDegrees( SLFloat x ) { return x * SL_DEGS_PER_RAD; }

void floatingPointLimitTest();

#endif // __SLMATH_H__
