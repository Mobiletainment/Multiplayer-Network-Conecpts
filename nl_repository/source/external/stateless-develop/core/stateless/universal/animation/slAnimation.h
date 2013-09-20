
#pragma once
#ifndef __SLANIMATION_H__
#define __SLANIMATION_H__
//!---------------------------------------------------------------------------
//! \file "slAnimation.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------



//#include "slFrameTime.h"
#include <Stateless/universal/time/slFrameTime.h>

namespace sl	{
	//------------------------------------------------------------------------------
	/// \brief represents a simple timed animation tick
	class AnimationTick
	{
	public:	
		AnimationTick();
		virtual ~AnimationTick();

		SL_IMPLEMENT_CLASS_COPY( AnimationTick )

		SLSize getAnimationTicks( void ) const { return _timeInfo.m_animationTick; }

		SLTimeInterval getAccumTime( void ) const { return _frameTime.m_accumTime; }
		SLTimeInterval getTickedAccumTime( void ) const { return _timeInfo.m_frameTime.m_tickedAccumTime; }

		void setAccumTime( SLTimeInterval value );

		SLTimeInterval getAnimationInterval( void ) const { return _dtAnim; }

		void setAnimationInterval( SLTimeInterval dtAnim );
		void setAnimationFrequency( SLDouble frequency );

		SLSize update( SLTimeInterval dt, bool& animationTickChanged );
		SLSize update( SLTimeInterval dt );

		// special use case
		void accumulate( SLTimeInterval dt );
		SLSize update( bool& animationTickChanged );
		SLSize getLastAnimationTick() const {return _lastAnimationTick;};
		virtual void reset( void ) SL_VIRTUAL_DECLARE;
	protected:
		void copyFrom( const AnimationTick& other );

		SLSimulationTimeInfo _timeInfo;
		SLFrameTime _frameTime;
		SLTimeInterval _dtAnim;
		SLSize _lastAnimationTick;
	};

	SL_INLINE
	void AnimationTick::setAnimationInterval( SLTimeInterval dtAnim )	{ 
		_dtAnim = dtAnim; 
	}

	SL_INLINE
	void AnimationTick::setAnimationFrequency( SLDouble frequency )	{
		frequency = frequency <= 0 ? 1000 : frequency;
		setAnimationInterval(1.0/frequency);
	}


	class AnimationBlendTick : public AnimationTick	{
		SL_DECLARE_BASE(AnimationTick)
	public:
		AnimationBlendTick();
		virtual ~AnimationBlendTick();

		SL_IMPLEMENT_CLASS_COPY( AnimationBlendTick )

		virtual void reset( void ) SL_OVERRIDE;

		SLSize updateBlendFactor( SLTimeInterval dt );

		SLDouble getBlendFactor( void ) const;

		SLDouble getBlendFactor( SLTimeInterval frameTimeDt ) const;
	protected:
		void copyFrom( const AnimationBlendTick& other );

	private:
		SLDouble _blendFactor;
	};

}




#endif
