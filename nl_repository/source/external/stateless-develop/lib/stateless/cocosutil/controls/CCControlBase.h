//!---------------------------------------------------------------------------
//! \file "CCControlBase.h"
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
#ifndef __CCCONTROLBASE_H__
#define __CCCONTROLBASE_H__

#include "GUI/CCControlExtension/CCControl.h"
#include <stateless/cocos2d/slCocosMacros.h>

#define NL_SUBLAYER_FRAME_WIDTH 10.0f
#define NL_MAINLAYER_FRAME_WIDTH 50.0f
#define NL_MAINLAYER_FRAME_HEIGHT 50.0f




namespace sl	{



	enum ECtrlFlags
	{
		ECtrlFlag_Undefined = 0,
		ECtrlFlag_DrawFocusRect = 1 << 0,
		ECtrlFlag_UseScissorTest = 1 << 1,
		ECtrlFlag_001 = 1 << 2,
		ECtrlFlag_002 = 1 << 3,
		ECtrlFlag_003 = 1 << 4,
		ECtrlFlag_004 = 1 << 5,
		ECtrlFlag_005 = 1 << 6,
		ECtrlFlag_006 = 1 << 7,
	};

	enum ECtrlLayoutFlags
	{
		ECtrlLayoutFlag_Undefined = 0,
		ECtrlLayoutFlag_TopDown = 1 << 0,
		ECtrlLayoutFlag_LeftRight = 1 << 1,
		ECtrlLayoutFlag_ResetPreferredSizeOnSerialize = 1 << 2,
		ECtrlLayoutFlag_002 = 1 << 3,
		ECtrlLayoutFlag_003 = 1 << 4,
		ECtrlLayoutFlag_004 = 1 << 5,
		ECtrlLayoutFlag_005 = 1 << 6,
		ECtrlLayoutFlag_006 = 1 << 7,
	};

#pragma warning(push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
	class slCocosUtil_API CtrlFlags0	{

	};

	class slCocosUtil_API CtrlFlags : public sltl::FlagSetMixin<CtrlFlags0, ECtrlFlags>	{

	};

	class slCocosUtil_API CtrlLayoutFlags0	{

	};

	class slCocosUtil_API CtrlLayoutFlags : public sltl::FlagSetMixin<CtrlLayoutFlags0, ECtrlLayoutFlags>	{

	};
#pragma warning(pop)


	enum ELayoutMode	{
		ELayoutMode_topLeftDown
	};

	class slCocosUtil_API AbstractControl : public cocos2d::extension::CCControl
	{
		SL_DECLARE_BASE(cocos2d::extension::CCControl)
	public:
		AbstractControl()
			:_preferredSize(CCSizeMake(0,0))
			,_maxSize(CCSizeMake(0,0))
			,_minSize(CCSizeMake(20,20))
		{

		}
		SL_DECLARE_ABSTRACT_CLASSNAME

		CC_SYNTHESIZE_PASS_BY_REF(CCSize, _preferredSize, PreferredSize)
		CC_SYNTHESIZE_PASS_BY_REF(CCSize, _maxSize, MaxSize)
		CC_SYNTHESIZE_PASS_BY_REF(CCSize, _minSize, MinSize)

		SL_SYNTHESIZE_GET_BY_REF(CtrlFlags, _ctrlFlags, CtrlFlags)
		SL_SYNTHESIZE_GET_BY_REF(CtrlLayoutFlags, _ctrlLayoutFlags, CtrlLayoutFlags)

	};

	class slCocosUtil_API CCControlBase : public AbstractControl
	{
		SL_DECLARE_BASE(AbstractControl)
	public:

		CREATE_FUNC(CCControlBase)

		SL_IMPLEMENT_CLASSNAME(CCControlBase)

		CCControlBase();

		bool initWithBackGroundSprite(
			CCScale9Sprite* backgroundSprite);

		bool initWithBackGroundSprite(
			const char* backgroundSpriteName);

		virtual bool init() SL_OVERRIDE;

		// note: the default implementation of needs layout is empty inside CCControl
		virtual void needsLayout() SL_OVERRIDE;

		// a hack actually
		virtual void redoLayout() SL_VIRTUAL_DECLARE;

		//! a tricky function you can implement empty if you do not need such feature
		//! e.g for a game play layer it's pretty much useless (maybe ...)
		virtual void layoutChildren(ELayoutMode layoutMode, bool resize = false) SL_VIRTUAL_DECLARE;

		//set the margins at once (so we only have to do one call of needsLayout)
		void setMargins(int marginH, int marginV);

		/* Define the button margin for Top/Bottom edge */
		CC_SYNTHESIZE_READONLY(int, m_marginV, VerticalMargin);
		/* Define the button margin for Left/Right edge */
		CC_SYNTHESIZE_READONLY(int, m_marginH, HorizontalMargin);

		CC_SYNTHESIZE(CCScale9Sprite*, _backGroundSprite, BackGroundSprite)
		CC_SYNTHESIZE(CCDictionary*, _dataSource, DataSource)
		CC_PROPERTY_READONLY(CCObject*, _ctrlValue, CtrlValue)

		bool canSerializeTo( CCDictionary* target ) const;
		bool canDeserializeFrom( CCDictionary* source ) const;
		void setValueKey( const char* key);
		const char* getValueKey() const;

		virtual void serialize( CCDictionary* target ) SL_VIRTUAL_DECLARE;
		virtual void deserialize( CCDictionary* source ) SL_VIRTUAL_DECLARE;

		virtual void serializeAll( CCDictionary* target ) SL_VIRTUAL_DECLARE;
		virtual void deserializeAll( CCDictionary* source, bool redoLayout = false ) SL_VIRTUAL_DECLARE;

		// CCNode interface
		virtual void draw() SL_OVERRIDE;
		virtual void visit() SL_OVERRIDE;

		virtual void setContentSize(const CCSize& contentSize) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		// CCTouchDelegate interface touch events
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) SL_OVERRIDE;
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) SL_OVERRIDE;

		// custom interface
		virtual void onAppResized(CCObject* app);

		virtual void addSubLayerContent();


		CCRect getWorldViewRect();

		void removeBackgroundSprite();

	protected:
		virtual ~CCControlBase();

		void registerResizeObserver();
		void deregisterResizeObserver();

		void drawFrame(const ccColor4B& color, float lineWidth) const;


	private:
		CCString* _valueKey;
		bool _scissorRestored;
	};


	template class slCocosUtil_API std::allocator<CCControlBase*>;
	typedef std::vector<CCControlBase*, std::allocator<CCControlBase*>> TCCControlBaseArray;

	class slCocosUtil_API CCControlBaseArray : private TCCControlBaseArray	{
	public:
		CCControlBaseArray();
		virtual ~CCControlBaseArray();

		CCControlBase* getControlAt(SLSize idx) const;

		void setSelected( bool value	);

		void add(CCControlBase* ctrl);

		void remove(CCControlBase* ctrl);
	};
}

#endif
