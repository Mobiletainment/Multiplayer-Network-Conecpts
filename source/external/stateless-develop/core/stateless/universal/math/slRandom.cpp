//!---------------------------------------------------------------------------
//! \file "slRandom.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


/*
 *  slRandom.cpp
 *
 *  Created by Cyrus Preuss on 1/7/09.
 *  Copyright 2009 Cyrus Preuss. All rights reserved.
 *
 */


#include "slRandom.h"
#if 0
#include "slFrameTime.h"
#endif
//-----------------------------------------------------------------------------


#define SL_USE_ARC4RANDOM 0

namespace sl	{
	//-----------------------------------------------------------------------------
	void randomSeed(unsigned seed)
	{
		srand(seed);
	}

	//-----------------------------------------------------------------------------
	void randomInit( void )
	{
#if 0
		SLTimeInterval currentTime = SLAbsoluteTimeGetCurrent();
		slRandomSeed((unsigned)currentTime);
#endif
	}

	//-----------------------------------------------------------------------------
	int randomInt(void)
	{
#if SL_USE_ARC4RANDOM
		return (int)(arc4random() % ((unsigned)RAND_MAX + 1));
#else
		return rand();
#endif
	}

	//-----------------------------------------------------------------------------
	bool randomYesNo( void )
	{
		return ( randomInt() % 2 > 0 ) ? true : false;
	}

	//-----------------------------------------------------------------------------
	SLFloat random01 (void)
	{
		return (((float) randomInt()) / ((float) RAND_MAX));
	}

	//-----------------------------------------------------------------------------
	SLFloat randomLowerUpper (SLFloat lowerBound, SLFloat upperBound)
	{
		if (lowerBound > upperBound) {
			const SLFloat temp(upperBound);
			upperBound = lowerBound;
			lowerBound = temp;
		}
		else if( lowerBound == upperBound) {
			return lowerBound;
		}
		return lowerBound + (random01 () * (upperBound - lowerBound));
	}

	//-----------------------------------------------------------------------------
	int randomIntLowerUpper( int lowerBound, int upperBound)
	{
		if (lowerBound > upperBound) {
			const int temp = upperBound;
			upperBound = lowerBound;
			lowerBound = temp;
		}
		else if( lowerBound == upperBound) {
			return lowerBound;
		}
		return lowerBound + (randomInt() % ((upperBound - lowerBound) + 1));
	}

}
