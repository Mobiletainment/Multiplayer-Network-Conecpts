//!---------------------------------------------------------------------------
//! \file "slRandom.h"
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
#ifndef __SLRANDOM_H__
#define __SLRANDOM_H__

/*
 *  slRandom.h
 *
 *  Created by Cyrus Preuss on 1/7/09.
 *  Copyright 2009 Cyrus Preuss. All rights reserved.
 *
 */

#include "slMath.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace sl	{
	/// initialize the random number generator with the specified seed
	void randomSeed(unsigned seed);

	/// initialize the random number generator based on the current time
	void randomInit( void );

	/// random true false
	bool randomYesNo( void );

	/// Returns a float randomly distributed between 0 and 1
	SLFloat random01 (void);

	/// Returns a float randomly distributed between lowerBound and upperBound
	SLFloat randomLowerUpper (SLFloat lowerBound, SLFloat upperBound);

	/// Returns an integer randomly distributed between lowerBound and upperBound
	int randomIntLowerUpper( int lowerBound, int upperBound);

	/// Returns an integer randomly distributed between -RAND_MAX and RAND_MAX
	int	randomInt(void);
}


#ifdef __cplusplus
}
#endif


#endif // __SLRANDOM_H__
