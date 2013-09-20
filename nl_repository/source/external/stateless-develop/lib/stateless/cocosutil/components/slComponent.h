//!---------------------------------------------------------------------------
//! \file "slComponent.h"
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
#ifndef __SLCOMPONENT_H__
#define __SLCOMPONENT_H__

#include "../slCCSmartPointer.h"

namespace sl	{

	class slCocosUtil_API IComponent	{
	public:
		SL_DECLARE_ABSTRACT_CLASSNAME;

		virtual bool isValid() const SL_ABSTRACT;

		virtual bool init() SL_ABSTRACT;
		virtual void onEnter() SL_ABSTRACT;
		virtual void onExit() SL_ABSTRACT;

		virtual void update(float delta) SL_ABSTRACT;

		virtual void preUpdate( float delta ) SL_ABSTRACT;
		virtual void postUpdate( float delta ) SL_ABSTRACT;

		virtual void visit() SL_ABSTRACT;
		virtual void draw() SL_ABSTRACT;

		virtual void serialize( CCDictionary* target ) const SL_ABSTRACT;
		virtual bool isEnabled() const SL_ABSTRACT;
		virtual void setEnabled(bool b) SL_ABSTRACT;

		virtual const char* getName() const SL_ABSTRACT;

		virtual void setOwner(CCNode *owner) SL_ABSTRACT;
		virtual CCNode* getOwner() const SL_ABSTRACT;

	protected:
		~IComponent()	{

		}
	};

	class slCocosUtil_API IComponentArray	{
	public:
		SL_DECLARE_ABSTRACT_CLASSNAME;

		virtual void preUpdate( float delta ) SL_ABSTRACT;
		virtual void postUpdate( float delta ) SL_ABSTRACT;


		virtual void serialize( CCDictionary* target ) const SL_ABSTRACT;
		virtual bool isEnabled() const SL_ABSTRACT;
		virtual void setEnabled(bool b) SL_ABSTRACT;

		virtual const char* getName() const SL_ABSTRACT;

		virtual void setOwner(CCNode *owner) SL_ABSTRACT;
		virtual CCNode* getOwner() const SL_ABSTRACT;

		virtual void addComponent(IComponent* component) SL_ABSTRACT;

		virtual bool removeComponent(IComponent* component) SL_ABSTRACT;

		virtual SLSize count() const SL_ABSTRACT;

		virtual IComponent* componentAt(SLSize idx) const SL_ABSTRACT;

	protected:
		~IComponentArray()	{

		}
	};

	class slCocosUtil_API Component: public CCObject, public IComponent	{
		SL_DECLARE_BASE(CCObject)
	public:
		CREATE_FUNC(Component)

		virtual bool isValid() const SL_OVERRIDE;

		SLSize idx() const;

		SL_IMPLEMENT_CLASSNAME(Component)

		virtual bool init() SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;

		virtual void onExit() SL_OVERRIDE;

		virtual void update( float delta ) SL_OVERRIDE;

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void visit() SL_OVERRIDE;
		virtual void draw() SL_OVERRIDE;

		virtual void serialize( CCDictionary* target ) const SL_OVERRIDE;

		virtual bool isEnabled() const SL_OVERRIDE;

		virtual void setEnabled( bool b ) SL_OVERRIDE;

		virtual const char* getName() const SL_OVERRIDE;

		virtual void setOwner( CCNode *owner ) SL_OVERRIDE;

		virtual CCNode* getOwner() const SL_OVERRIDE;

	protected:
		Component();
		~Component();

	private:
		SLSize _idx;
		CCNode *_owner;
		bool _enabled;
#pragma warning (push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
		static InstanceCount _instanceCount;
#pragma warning (pop)
	};

	SL_INLINE
	SLSize Component::idx()	const {
		return _idx;
	}

	typedef sltl::TCCSmartPointer<Component> ComponentPtr;

	typedef std::vector<ComponentPtr> TComponentPtrArray;

	class slCocosUtil_API ComponentArray: public CCNode, public IComponentArray	{
		SL_DECLARE_BASE(CCNode)
	public:
		static void testComponentArray();

		CREATE_FUNC(ComponentArray)

		SL_IMPLEMENT_CLASSNAME(ComponentArray)

		virtual bool init() SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;

		virtual void onExit() SL_OVERRIDE;

		virtual void update( float delta ) SL_OVERRIDE;

		virtual void preUpdate( float delta ) SL_OVERRIDE;
		virtual void postUpdate( float delta ) SL_OVERRIDE;

		virtual void visit() SL_OVERRIDE;
		virtual void draw() SL_OVERRIDE;

		virtual void serialize( CCDictionary* target ) const SL_OVERRIDE;

		virtual bool isEnabled() const SL_OVERRIDE;

		virtual void setEnabled( bool b ) SL_OVERRIDE;

		virtual const char* getName() const SL_OVERRIDE;

		virtual void setOwner( CCNode *owner ) SL_OVERRIDE;

		virtual CCNode* getOwner() const SL_OVERRIDE;

		virtual void addComponent(IComponent* component) SL_OVERRIDE;

		virtual bool removeComponent(IComponent* component) SL_OVERRIDE;

		virtual SLSize count() const SL_OVERRIDE;

		virtual IComponent* componentAt(SLSize idx) const SL_OVERRIDE;


		void clear();

	protected:
		ComponentArray();
		~ComponentArray();

#pragma warning (push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
		TComponentPtrArray _components;
#pragma warning (pop)
		bool _enabled;

	};


}

#endif
