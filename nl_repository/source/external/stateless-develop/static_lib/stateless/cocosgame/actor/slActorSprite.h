//!---------------------------------------------------------------------------
//! \file "slActorSprite.h"
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
#ifndef __SLACTORSPRITE_H__
#define __SLACTORSPRITE_H__

#include "slGameActor.h"

namespace sl	{


	class slCocosGame_API ActorSprite : public VehicleActor
	{
		SL_DECLARE_BASE(VehicleActor)
	public:
		static ActorSprite* createWithDictionary(CCDictionary* dictionary);

		virtual bool initWithDictionary(CCDictionary* dictionary);

		virtual void update(float dt);

		virtual GameplayLayer* getGameplayLayer() const SL_OVERRIDE;

		void setGamePosition(const CCPoint& gamePosition);

		CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint, _gamePosition, GamePosition)
	protected:
		ActorSprite();
		virtual ~ActorSprite();

		virtual void updateVisualFromGamePosition(const CCPoint &position);
	private:
		void annotateSprite(float dt, bool annotate);
		void updateSinglePlayer(float dt);

		Vec3 _lastForward;
		Vec3 _lastPosition;
		SLTimeInterval _accumTime;
		CCLabelTTF* _label;
	};


}


#endif // __SLACTORSPRITE_H__
