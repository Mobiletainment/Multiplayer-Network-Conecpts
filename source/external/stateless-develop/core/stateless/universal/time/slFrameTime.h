#pragma once
#ifndef __SLFRAMETIME_H__
#define __SLFRAMETIME_H__
//!---------------------------------------------------------------------------
//! \file slFrameTime.h
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef __SLCORETYPES_H__
#  include "slCoreTypes.h"
#endif


//------------------------------------------------------------------------------
/// This structure is used to capture single frame times
typedef struct SLFrameTime
{
	SLTimeInterval m_lastTime;
	SLTimeInterval m_deltaTime;
	SLTimeInterval m_accumTime;
	SLTimeInterval m_tickedAccumTime;
} SLFrameTime;

//------------------------------------------------------------------------------
/// This structure is used to capture animation ticks for specific frametimes
typedef struct SLSimulationTimeInfo
{
	SLFrameTime m_frameTime;
	SLSize m_animationTick;
} SLSimulationTimeInfo;



/// initialize the frame time structure
static void SLInitFrameTime( SLFrameTime* frameTime );

/// update the frame time structure
static void SLUpdateFrameTime( SLFrameTime* frameTime, SLTimeInterval currTime );

/// update the frame time structure only accept a certain max delta time
static void SLUpdateFrameTimeWithMaxDelta( SLFrameTime* frameTime, SLTimeInterval maxDt, SLTimeInterval currTime );

static void SLUpdateAnimationTicksWithTime(SLSimulationTimeInfo* simulationTime,
	SLTimeInterval deltaTime,
	SLTimeInterval dtAnim);

static SLTimeInterval SLGetDtForFrameRate( SLDouble frameRate );

#ifdef __cplusplus
extern "C" {
#endif

	SLTimeInterval SLAbsoluteTimeGetCurrent();


#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
static SL_INLINE
void SLUpdateAnimationTicksWithTime(SLSimulationTimeInfo* simulationTime,
									SLTimeInterval deltaTime,
									SLTimeInterval dtAnim)
{
	// accumulate time
	simulationTime->m_frameTime.m_deltaTime += deltaTime;
	simulationTime->m_frameTime.m_accumTime += deltaTime;
	if (dtAnim > 0.0) {
		// increment animation ticks
		while (simulationTime->m_frameTime.m_deltaTime >= dtAnim) {
			++simulationTime->m_animationTick;
			simulationTime->m_frameTime.m_tickedAccumTime += dtAnim;
			simulationTime->m_frameTime.m_deltaTime -= dtAnim;
		}
	}
}

//------------------------------------------------------------------------------
static SL_INLINE
void SLInitFrameTime( SLFrameTime* frameTime )
{
	frameTime->m_lastTime =
	frameTime->m_deltaTime =
	frameTime->m_accumTime = 
	frameTime->m_tickedAccumTime = 0.0;
}

//------------------------------------------------------------------------------
static SL_INLINE
void SLUpdateFrameTime( SLFrameTime* frameTime,
					   SLTimeInterval currTime
					   )
{
	if (frameTime->m_lastTime == 0.0) {
		frameTime->m_lastTime = currTime;
	}
	else	{
		frameTime->m_deltaTime = currTime - frameTime->m_lastTime;
		frameTime->m_lastTime = currTime;
		frameTime->m_accumTime += frameTime->m_deltaTime;
		frameTime->m_tickedAccumTime += frameTime->m_deltaTime;
	}
}

//------------------------------------------------------------------------------
static SL_INLINE
void SLUpdateFrameTimeWithMaxDelta( SLFrameTime* frameTime,
								   SLTimeInterval maxDt,
								   SLTimeInterval currTime
								   )
{
	if (frameTime->m_lastTime == 0.0) {
		frameTime->m_lastTime = currTime;
	}
	else	{
		const SLTimeInterval dt = (currTime - frameTime->m_lastTime);
		// note: prevent negative deltatimes at any case
		if (dt > 0.0) {
			frameTime->m_deltaTime = dt;
			if (frameTime->m_deltaTime > maxDt) {
				frameTime->m_deltaTime = maxDt;
			}
			frameTime->m_lastTime = currTime;
			frameTime->m_accumTime += frameTime->m_deltaTime;
			frameTime->m_tickedAccumTime += frameTime->m_deltaTime;
		}
	}
}

//------------------------------------------------------------------------------
static SL_INLINE
SLTimeInterval SLGetDtForFrameRate( SLDouble frameRate )
{
	if (frameRate > 0.0) {
		return 1.0 / frameRate;
	}
	return 0.001; // 1000 Hz should be fine for most simulations
}

#endif // __SLFRAMETIME_H__
