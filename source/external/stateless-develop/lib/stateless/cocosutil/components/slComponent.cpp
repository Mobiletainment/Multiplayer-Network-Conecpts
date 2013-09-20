//!---------------------------------------------------------------------------
//! \file "slComponent.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "slComponent.h"

namespace sl	{

	//-------------------------------------------------------------------------
	// Component
	//-------------------------------------------------------------------------

	InstanceCount Component::_instanceCount;

	Component::Component()
		:_enabled(true)
		,_owner(nullptr)
	{
		_idx = _instanceCount.constructor();
	}

	Component::~Component()
	{
		SLSize destructorCount = _instanceCount.destructor();
		if(destructorCount == _instanceCount.getConstructedCount())	{
			bool test(true);
			test = false;
		}
	}

	bool Component::init()
	{
		return true;
	}


	void Component::onEnter() 
	{
	}

	void Component::onExit() 
	{
	}

	void Component::update( float delta ) 
	{
	}

	void Component::preUpdate( float delta ) 
	{
	}

	void Component::postUpdate( float delta ) 
	{
	}

	void Component::visit()
	{

	}

	void Component::draw()
	{

	}

	void Component::serialize(  CCDictionary* target ) const 
	{
		// implement this if you want to call it
		assert(false);
	}

	bool Component::isEnabled() const 
	{
		return _enabled;
	}

	void Component::setEnabled( bool b ) 
	{
		if(b != _enabled)	{
			_enabled = b;
		}
	}

	const char* Component::getName() const 
	{
		return getClassName();
	}

	void Component::setOwner( CCNode *owner )
	{
		_owner = owner;
	}

	CCNode* Component::getOwner() const
	{
		return _owner;
	}

	bool Component::isValid() const
	{
		return true;
	}

	//-------------------------------------------------------------------------
	// ComponentArray
	//-------------------------------------------------------------------------
	ComponentArray::ComponentArray()
		:_enabled(true)
	{

	}

	ComponentArray::~ComponentArray()
	{

	}

	void ComponentArray::testComponentArray()
	{
		TComponentPtrArray components;

		for(SLSize i(0); i < 1000; ++i)	{
			components.push_back(ComponentPtr::createInstance());
		}

		components.clear();

	}

	bool ComponentArray::init() 
	{
		return SLBaseClass::init();
	}

	void ComponentArray::onEnter() 
	{
		SLBaseClass::onEnter();
	}

	void ComponentArray::onExit() 
	{
		SLBaseClass::onExit();
	}

	void ComponentArray::update( float delta ) 
	{
		SLBaseClass::update(delta);
		TComponentPtrArray::iterator iter(_components.begin());
		TComponentPtrArray::iterator iterEnd(_components.end());
		while(iter != iterEnd)	{
			++iter;
		}
	}

	void ComponentArray::preUpdate( float delta ) 
	{
		SLBaseClass::update(delta);
		TComponentPtrArray::iterator iter(_components.begin());
		TComponentPtrArray::iterator iterEnd(_components.end());
		while(iter != iterEnd)	{
			(*iter)->preUpdate(delta);
			++iter;
		}
	}

	void ComponentArray::postUpdate( float delta ) 
	{
		SLBaseClass::update(delta);
		TComponentPtrArray::iterator iter(_components.begin());
		TComponentPtrArray::iterator iterEnd(_components.end());
		while(iter != iterEnd)	{
			(*iter)->postUpdate(delta);
			++iter;
		}
	}


	void ComponentArray::visit()
	{
		SLBaseClass::visit();
		TComponentPtrArray::iterator iter(_components.begin());
		TComponentPtrArray::iterator iterEnd(_components.end());
		while(iter != iterEnd)	{
			++iter;
		}
	}

	void ComponentArray::draw()
	{
		SLBaseClass::draw();
		TComponentPtrArray::iterator iter(_components.begin());
		TComponentPtrArray::iterator iterEnd(_components.end());
		while(iter != iterEnd)	{
			++iter;
		}
	}

	void ComponentArray::serialize(  CCDictionary* target ) const 
	{
	}

	bool ComponentArray::isEnabled() const 
	{
		return _enabled;
	}

	void ComponentArray::setEnabled( bool b ) 
	{
		_enabled = b;
	}

	const char* ComponentArray::getName() const 
	{
		return getClassName();
	}

	void ComponentArray::setOwner( CCNode *owner ) 
	{
		if(owner != nullptr)	{
			owner->addChild(this);
		}
		else	{
			CCNode *currentOwner(getOwner());
			if(currentOwner != nullptr)	{
				removeFromParent();
			}
		}
	}

	CCNode* ComponentArray::getOwner() const 
	{
//		CCNode* thisPtr((CCNode*)this);
		return getParent();
	}

	void ComponentArray::addComponent( IComponent* component )
	{
		Component* c(dynamic_cast<Component*>(component));
		if(c != nullptr)	{
			if(std::find(_components.begin(), _components.end(), c) == _components.end())	{
				_components.push_back(c);
			}
		}
		else	{
			SL_PROCESS_APP()->log(ELogType_Error, "failed adding a component name(%s)", component->getName());
		}
	}


	void ComponentArray::clear()	{
		_components.clear();
	}

	SLSize ComponentArray::count() const
	{
		return _components.size();
	}

	IComponent* ComponentArray::componentAt( SLSize idx ) const	{
		if(idx < count())	{
			return _components[idx];
		}
		return nullptr;
	}

	bool ComponentArray::removeComponent( IComponent* component )
	{
		// find IComponent in container
		const TComponentPtrArray::iterator s = std::find (_components.begin(), _components.end(), component);
		if(s != _components.end())	{
			_components.erase(s);
			return true;
		}

		return false;
	}

}
