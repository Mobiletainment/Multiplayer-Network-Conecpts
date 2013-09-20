//!---------------------------------------------------------------------------
//! \file "slAnimation.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slAnimation.h"

namespace sl	{
	//------------------------------------------------------------------------------
	AnimationTick::AnimationTick()
		:_dtAnim(0.0)
		,_lastAnimationTick(0)
	{
		_timeInfo.m_animationTick = 0;
		SLInitFrameTime( &_frameTime );
		SLInitFrameTime( &_timeInfo.m_frameTime );
	}

	//------------------------------------------------------------------------------
	AnimationTick::~AnimationTick()	{
	}

	//------------------------------------------------------------------------------
	void AnimationTick::copyFrom( const AnimationTick& other )
	{
		_frameTime = other._frameTime;
		_timeInfo = other._timeInfo;
		_dtAnim = other._dtAnim;
		_lastAnimationTick = other._lastAnimationTick;
	}

	//------------------------------------------------------------------------------
	void AnimationTick::setAccumTime( SLTimeInterval value )	{
		const SLTimeInterval currDtAnim(_dtAnim);
		reset();
		bool animationTickChanged(false);
		update(value, animationTickChanged);
		setAnimationInterval(currDtAnim);
	}

	//------------------------------------------------------------------------------
	SLSize AnimationTick::update( SLTimeInterval dt, bool& animationTickChanged )	{
		const SLSize currentTick(_timeInfo.m_animationTick);
		_lastAnimationTick = _timeInfo.m_animationTick;
		_frameTime.m_lastTime = _frameTime.m_accumTime;
		_frameTime.m_deltaTime = dt;
		_frameTime.m_accumTime += _frameTime.m_deltaTime;
		SLUpdateAnimationTicksWithTime( &_timeInfo, _frameTime.m_deltaTime, _dtAnim );
		animationTickChanged = (currentTick != _timeInfo.m_animationTick);
		return _timeInfo.m_animationTick;
	}

	//------------------------------------------------------------------------------
	SLSize AnimationTick::update( SLTimeInterval dt )	{
		_lastAnimationTick = _timeInfo.m_animationTick;
		_frameTime.m_lastTime = _frameTime.m_accumTime;
		_frameTime.m_deltaTime = dt;
		_frameTime.m_accumTime += _frameTime.m_deltaTime;
		SLUpdateAnimationTicksWithTime( &_timeInfo, _frameTime.m_deltaTime, _dtAnim );
		return _timeInfo.m_animationTick;
	}

	//------------------------------------------------------------------------------
	void AnimationTick::accumulate( SLTimeInterval dt )	{
		_timeInfo.m_frameTime.m_deltaTime += dt;
		_timeInfo.m_frameTime.m_accumTime += dt;
		_frameTime.m_accumTime += _frameTime.m_deltaTime;
	}

	//------------------------------------------------------------------------------
	SLSize AnimationTick::update( bool& animationTickChanged )	{
		_frameTime.m_lastTime = _frameTime.m_accumTime;
		_frameTime.m_deltaTime = 0;
		SLUpdateAnimationTicksWithTime( &_timeInfo, 0, _dtAnim );
		return _timeInfo.m_animationTick;
	}

	//------------------------------------------------------------------------------
	void AnimationTick::reset( void )
	{
		SLInitFrameTime( &_frameTime );
		SLInitFrameTime( &_timeInfo.m_frameTime );
		_timeInfo.m_animationTick = 0;
		_lastAnimationTick = 0;
		_dtAnim = 0.0;
	}


	//------------------------------------------------------------------------------
	AnimationBlendTick::AnimationBlendTick()
		:_blendFactor(0.0)
	{
	}

	//------------------------------------------------------------------------------
	AnimationBlendTick::~AnimationBlendTick()	{
	}

	//------------------------------------------------------------------------------
	void AnimationBlendTick::copyFrom( const AnimationBlendTick& other )
	{
		SLBaseClass::copyFrom(other);
		_blendFactor = other._blendFactor;
	}

	//------------------------------------------------------------------------------
	void AnimationBlendTick::reset( void )
	{
		SLBaseClass::reset();
		_blendFactor = 0.0;
	}

	//------------------------------------------------------------------------------
	SLSize AnimationBlendTick::updateBlendFactor( SLTimeInterval dt )	{
		_frameTime.m_lastTime = _frameTime.m_accumTime;
		_frameTime.m_deltaTime = dt;
		_frameTime.m_accumTime += _frameTime.m_deltaTime;
		SLTimeInterval frameTimeDt(_timeInfo.m_frameTime.m_deltaTime + _frameTime.m_deltaTime);
		_blendFactor = getBlendFactor(frameTimeDt);
		SLUpdateAnimationTicksWithTime( &_timeInfo, _frameTime.m_deltaTime, _dtAnim );
		return _timeInfo.m_animationTick;
	}

#ifndef MIN
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

	//------------------------------------------------------------------------------
	SLDouble AnimationBlendTick::getBlendFactor( SLTimeInterval frameTimeDt ) const	{
		if (_dtAnim > 0.0) {
			if( frameTimeDt <= 0.0 )
			{
				return 0.0f;
			}
			else	{
				SLDouble blendFactor( frameTimeDt / _dtAnim );
				return MIN(1.0, blendFactor);
			}
		}
		return 0.0f;
	}

	//------------------------------------------------------------------------------
	SLDouble AnimationBlendTick::getBlendFactor( void ) const	{
		return _blendFactor;
	}

}
