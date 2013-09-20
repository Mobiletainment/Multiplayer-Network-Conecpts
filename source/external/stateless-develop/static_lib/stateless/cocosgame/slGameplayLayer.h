//!---------------------------------------------------------------------------
//! \file "slGameplayLayer.h"
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
#ifndef __SLGAMEPLAYLAYER_H__
#define __SLGAMEPLAYLAYER_H__

#include <stateless/simulation/slOpenSteer.h>
#include "slLocalPlayer.h"

namespace OpenSteer	{
	class Plugin;
}

namespace sl	{

	class ActorSprite;

	enum EGameplayLayerFlags
	{
		EGameplayLayerFlag_Undefined = 0,
		EGameplayLayerFlag_HasPath = 1 << 0,
		EGameplayLayerFlag_DrawText = 1 << 1,
		EGameplayLayerFlag_001 = 1 << 2,
		EGameplayLayerFlag_002 = 1 << 3,
		EGameplayLayerFlag_003 = 1 << 4,
		EGameplayLayerFlag_004 = 1 << 5,
		EGameplayLayerFlag_005 = 1 << 6,
		EGameplayLayerFlag_006 = 1 << 7,
	};

#pragma warning(push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
	class slCocosGame_API GameplayLayerFlags0	{

	};

	class slCocosGame_API GameplayLayerFlags : public sltl::FlagSetMixin<GameplayLayerFlags0, EGameplayLayerFlags>	{

	};

#pragma warning(pop)


	class slCocosGame_API GameplayLayer : public CCControlBase	{
		SL_DECLARE_BASE(CCControlBase)
	public:
		CREATE_FUNC(GameplayLayer)

		SL_IMPLEMENT_CLASSNAME(GameplayLayer)

		virtual bool init() SL_OVERRIDE;

		virtual void needsLayout() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;


		virtual void onKeyDown( CCObject* key );
		virtual void onKeyUp( CCObject* key );

		virtual void visit() SL_OVERRIDE;
		virtual void draw() SL_OVERRIDE;

		virtual bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ) SL_OVERRIDE;
		virtual void ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent ) SL_OVERRIDE;

		virtual void layoutChildren(ELayoutMode layoutMode, bool resize = false) SL_OVERRIDE;

		virtual void dispatchActorSpriteSinglePlayerAction( ActorSprite* actorSprite ) SL_VIRTUAL_DECLARE;

		void createPath();

		void setSelected( bool value );

		void drawLine(
			const Vec3& from, 
			const Vec3& to,
			const Color& color) const;

		SLSize layerIdx() const;

		void registerVehicle(AbstractVehicle* vehicle);
		void deregisterVehicle(AbstractVehicle* vehicle);

		const AVGroup& allVehicles (void) const; 
		AVGroup& allVehicles (void);


		static GameplayLayer* getGameplayLayer(CCObject* child);

		CC_SYNTHESIZE(Peer*, _peer, Peer)

		CC_SYNTHESIZE(Plugin*, _osPlugin, OSPlugin)

		SL_SYNTHESIZE_GET_BY_REF(LocalPlayer, _localPlayer, LocalPlayer)
		SL_SYNTHESIZE_GET_BY_REF(CCControlBaseArray, _serializeControls, SerializeControls)
		SL_SYNTHESIZE_GET(PolylineSegmentedPathwaySingleRadius*, _path, Path)
		SL_SYNTHESIZE_GET(ProximityDatabase*, _proximityDatabase, ProximityDatabase)
		SL_SYNTHESIZE_GET(PersistentDictionary*, _gameState, GameState)

		SL_SYNTHESIZE_GET_BY_REF(GameplayLayerFlags, _gamplayLayerFlags, GameplayLayerFlags)

	protected:
		GameplayLayer();
		virtual ~GameplayLayer();

		void addKeyObserver();
		void removeKeyObserver();

		void createProximityDatabase();
		void destroyProximityDatabase();

		virtual void updateSerialization(float dt) SL_VIRTUAL_DECLARE;

		static CCControlBaseArray _gameplayLayers;

		PluginArray _pluginArray;
		AVGroup _allVehicles;
		bool _drawVehicles;
	private:
		static InstanceCount _instanceCount;
		SLSize _layerIdx;
		SLTimeInterval _accumTime;
		AnimationTick _serializeCtrlsTick;
	};


	SL_INLINE
	SLSize GameplayLayer::layerIdx() const	{
		return _layerIdx;
	}

	/*
	typedef  sl::CCControlBase* (slFunc_createControl)( 
		const char* className, 
		const CCSize& preferredSize,
		CCScale9Sprite* backgroundSprite
		);
	*/

	CCControlBase* createGameplayLayer( 
		const char* className, 
		const CCSize& preferredSize,
		CCScale9Sprite* backgroundSprite
		);


}


#endif // __SLGAMEPLAYLAYER_H__
