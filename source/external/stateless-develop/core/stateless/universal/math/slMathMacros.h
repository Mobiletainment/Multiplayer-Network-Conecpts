#pragma once
#ifndef __SLMATHMACROS_H__
#define __SLMATHMACROS_H__

//!---------------------------------------------------------------------------
//! \file "slMathMacros.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "../core/slCoreTypes.h"

#ifdef __cplusplus
#  define SL_2_PI         SLFloat(6.283185307179586232f)
#  define SL_TWO_PI SL_2_PI
#  define SL_PI           (SL_2_PI * SLFloat(0.5f))
#  define SL_HALF_PI      (SL_2_PI * SLFloat(0.25f))
#  define SL_RADS_PER_DEG (SL_2_PI / SLFloat(360.0f))
#  define SL_DEGS_PER_RAD  (SLFloat(360.0) / SL_2_PI)

#  define SL_DEGTORAD(X) ((X) * SL_PI / 180.0f )
#  define SL_RADTODEG(X) ((X) * 180.0f /  SL_PI )
#else
#  define SL_2_PI         6.283185307179586232f
#  define SL_TWO_PI SL_2_PI
#  define SL_PI           (SL_2_PI * 0.5f)
#  define SL_HALF_PI      (SL_2_PI * 0.25f)
#  define SL_RADS_PER_DEG (SL_2_PI / 360.0f)
#  define SL_DEGS_PER_RAD  (360.0f / SL_2_PI)

#  define SL_DEGTORAD(X) ((X) * SL_PI / 180.0f )
#  define SL_RADTODEG(X) ((X) * 180.0f /  SL_PI )
#endif






#endif
