//!---------------------------------------------------------------------------
//! \file "slUpdatePeriod.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef __SLUPDATEPERIOD_H__
#define __SLUPDATEPERIOD_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2005-2013, Cyrus Preuss
//-----------------------------------------------------------------------------

#include <float.h>
#include <stddef.h>
#include <stdlib.h>

namespace sl	{
	template <class TFLOAT, class TMathLimits>
	struct TUpdatePeriod
	{
		TUpdatePeriod()
			:_accumTime( TFLOAT(0.0) )
			,_lastAccumTime( TFLOAT(0.0) )
			,_accumPeriodTime( TFLOAT(0.0) )
			,_periodTime( TFLOAT(-1.0) )
			,_periodFrequency(  TFLOAT(0.0) )
			,_loops(0)
		{
		}

		TUpdatePeriod( const TUpdatePeriod& other  )
		{
			copyFrom( other );
		}

		TUpdatePeriod& operator=( const TUpdatePeriod& other ){ copyFrom( other ); return (*this);}

		void copyFrom( const TUpdatePeriod& other  )
		{
			_lastAccumTime = ( TFLOAT(0.0) );
			_accumTime = ( TFLOAT(0.0) );
			_accumPeriodTime = ( TFLOAT(0.0) );
			_periodTime = ( other._periodTime );
			_periodFrequency = ( other._periodFrequency );
			_loops = (0);
		}

		TFLOAT getDeltaTime( size_t ticks ) const
		{
			return static_cast<TFLOAT>(ticks) * _periodTime;
		}

		void reset( void )
		{
			// do not reset the duration here
			// explicitly set it to the desired value
			_accumTime = TFLOAT(0.0);
			_lastAccumTime = TFLOAT(0.0);
			_accumPeriodTime = TFLOAT(0.0);
			_loops = 0;
		}

		TFLOAT getPeriodFrequency( void ) const
		{
			return _periodFrequency;
		}

		TFLOAT getPeriodTime( void ) const
		{
			return _periodTime;
		}

		TFLOAT getAccumTime( void ) const
		{
			return _accumTime;
		}

		void setPeriodFrequency( TFLOAT frequency,
			bool randomizeStart = false,
			TFLOAT randomFactor = TFLOAT( 0.75 ) );

		void setPeriodTime( TFLOAT periodTime,
			bool randomizeStart = false,
			TFLOAT randomFactor = TFLOAT( 0.75 ) );

		void setAccumPeriodTime( TFLOAT accumTime )
		{
			_accumPeriodTime = accumTime;
		}

		size_t updateAccumTime( TFLOAT accumTime )
		{
			size_t loops = 0;
			if( _periodTime <= TFLOAT(0.0) )
				return loops;

			if( _lastAccumTime == TFLOAT(0.0) )
			{
				// prevent initial overflow caused by 'huge'
				// initial accumulated times
				if( accumTime > TFLOAT( 0.5 ) )
				{
					_lastAccumTime = accumTime;
				}
			}
			TFLOAT deltaTime = accumTime - _lastAccumTime;
			// prevent deltatime overflow ?
			if( ( deltaTime > TFLOAT( 5.0 ) ) && ( deltaTime <= _periodTime ) )
			{
				deltaTime = _periodTime;
			}
			_lastAccumTime = accumTime;

			_accumPeriodTime += deltaTime;
			while( _accumPeriodTime >= _periodTime )
			{
				_accumPeriodTime -= _periodTime;
				_accumTime += _periodTime;
				++loops;
			}
			_loops += loops;
			return loops;
		}

		size_t updateDeltaTime( TFLOAT deltaTime )
		{
			TFLOAT fAccumTime = _lastAccumTime + deltaTime;
			return updateAccumTime( fAccumTime );
		}

		void randomizeStartTime( TFLOAT fRandomFactor = TFLOAT( 0.75 ) );

		TFLOAT remainingTime( void ) const
		{
			if( _periodTime > TFLOAT(0.0) )
			{
				if( periodElapsed(  ) )
				{
					return TFLOAT(0.0);
				}
				else
				{
					return _periodTime - _accumPeriodTime;
				}
			}
			else
			{
				return TFLOAT(0.0);
			}
		}

		TFLOAT accumPeriodTime( void ) const
		{
			return _accumPeriodTime;
		}

		TFLOAT period( void ) const
		{
			return getPeriodTime();
		}

		bool periodElapsed( void ) const
		{
			return (0 != _loops);
		}

	private:
		// Returns a btScalar randomly distributed between 0 and 1
		TFLOAT random01(void)
		{
			return (((TFLOAT) rand ()) / ((TFLOAT) RAND_MAX));
		}

		TFLOAT _lastAccumTime;
		TFLOAT _accumTime;
		TFLOAT _accumPeriodTime;
		TFLOAT _periodTime;
		TFLOAT _periodFrequency;
		size_t _loops;

	};

	//-----------------------------------------------------------------------------
	template <class TFLOAT, class TMathLimits>
	void TUpdatePeriod<TFLOAT, TMathLimits>::setPeriodFrequency(
		TFLOAT frequency,
		bool randomizeStart,
		TFLOAT randomFactor )
	{
		if( frequency == _periodFrequency )
		{
			return;
		}
		if( frequency > TFLOAT( 0.0 ) )
		{
			_periodTime = TFLOAT( 1.0 ) / frequency;
			_periodFrequency = frequency;
			if( randomizeStart == true )
			{
				randomizeStartTime( randomFactor );
			}
		}
		else
		{
			_periodFrequency =  TFLOAT( 0.0 );
			_periodTime = TMathLimits::getInfinity();
		}
	}

	//-----------------------------------------------------------------------------
	template <class TFLOAT, class TMathLimits>
	void TUpdatePeriod<TFLOAT, TMathLimits>::setPeriodTime( TFLOAT periodTime,
		bool randomizeStart,
		TFLOAT randomFactor )
	{
		if( periodTime == _periodTime )
		{
			return;
		}
		_periodTime = periodTime;
		if( _periodTime > TFLOAT( 0.0 ) )
		{
			_periodFrequency = TFLOAT( 1.0 ) / _periodTime;;
			if( randomizeStart == true )
			{
				randomizeStartTime( randomFactor );
			}
		}
		else
		{
			_periodFrequency = TMathLimits::getInfinity();
		}
	}

	//-----------------------------------------------------------------------------
	template <class TFLOAT, class TMathLimits>
	void TUpdatePeriod<TFLOAT, TMathLimits>::randomizeStartTime( TFLOAT randomFactor )
	{
		if( TFLOAT( 0.0 ) == _lastAccumTime )
		{
			// compute a random start offset related to the period time
			TFLOAT deltaTime = random01() * period() * randomFactor;
			updateDeltaTime( deltaTime );
		}
	}

	//-----------------------------------------------------------------------------
	class FloatMathLimits
	{
	public:
		static float getInfinity(){ return FLT_MAX; };
	};

	//-----------------------------------------------------------------------------
	class DoubleMathLimits
	{
	public:
		static double getInfinity(){ return DBL_MAX; };
	};

	typedef struct TUpdatePeriod<float, FloatMathLimits> UpdatePeriodFloat;
	typedef struct TUpdatePeriod<double, DoubleMathLimits> UpdatePeriodDouble;
}


#endif // __SLUPDATEPERIOD_H__
