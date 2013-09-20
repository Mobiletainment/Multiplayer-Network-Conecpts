//!---------------------------------------------------------------------------
//! \file "slPersistentDictionary.h"
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
#ifndef __SLPERSISTENTDICTIONARY_H__
#define __SLPERSISTENTDICTIONARY_H__

#include "slDictionary.h"

namespace sl	{

	class slCocosUtil_API AbstractPersistentDictionary : public Dictionary
	{
		SL_DECLARE_BASE(Dictionary)
	public:
		CC_SYNTHESIZE_RETAIN(CCDictionary*, _rootDictionary, RootDictionary)
	protected:
		AbstractPersistentDictionary();
		virtual ~AbstractPersistentDictionary();
	};

	class slCocosUtil_API PersistentDictionary : public AbstractPersistentDictionary
	{
		SL_DECLARE_BASE(AbstractPersistentDictionary)
	public:
		CREATE_FUNC(PersistentDictionary)

		virtual bool init() SL_OVERRIDE;

		virtual void update(float dt) SL_OVERRIDE;

		virtual void onEnter() SL_OVERRIDE;
		virtual void onExit() SL_OVERRIDE;

		virtual CCDictionary* getRootDictionary() const SL_OVERRIDE;

		void load();
		void save();

		void dumpInfo(void) const;

		CCDictionary* getSubDictionary(const char* key);

#pragma warning (push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
		CC_SYNTHESIZE(SLAString, _fileName, FileName)
#pragma warning (pop)
	protected:
		PersistentDictionary();
		virtual ~PersistentDictionary();

		const char* getSerializeFileName() const;
	private:
#pragma warning (push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
		SLAString _internalFileName;
		SLAString getWritablePath();
#pragma warning (pop)
	};

}


#endif
