//!---------------------------------------------------------------------------
//! \file "slPersistentDictionary.cpp"
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
#include "slPersistentDictionary.h"



namespace sl	{

	//-------------------------------------------------------------------------
	// AbstractPersistentDictionary
	//-------------------------------------------------------------------------
	AbstractPersistentDictionary::AbstractPersistentDictionary()
		:_rootDictionary(nullptr)
	{

	}

	AbstractPersistentDictionary::~AbstractPersistentDictionary()
	{
		setRootDictionary(nullptr);
	}


	//-------------------------------------------------------------------------
	// PersistentDictionary
	//-------------------------------------------------------------------------
	PersistentDictionary::PersistentDictionary()
	{
	}

	PersistentDictionary::~PersistentDictionary()	{
		save();
	}

	bool PersistentDictionary::init()	{
		bool initialized(SLBaseClass::init());
		if(initialized)	{
		}
		return initialized;
	}

	// TODO @CP : double check CCFileUtils::sharedFileUtils()->getWritablePath();

	SLAString PersistentDictionary::getWritablePath()	{
#ifdef WIN32
		// Get full path of executable, e.g. c:\Program Files (x86)\My Game Folder\MyGame.exe
		char full_path[_MAX_PATH + 1];
		::GetModuleFileNameA(NULL, full_path, _MAX_PATH + 1);
		// If fetching of local app data directory fails, use the executable one
		SLAString ret((char*)full_path);

		// remove xxx.exe
		ret =  ret.substr(0, ret.rfind("\\") + 1);

		return ret;
#else
		return CCFileUtils::sharedFileUtils()->getWritablePath();
#endif
	}

	void PersistentDictionary::load()	{
		if(_fileName.length() == 0)	{
			SL_PROCESS_APP()->log(ELogType_Error, "PersistentDictionary failed reading - filename is empty");
			return;
		}

		SLAString targetPath(PersistentDictionary::getWritablePath());
		targetPath += "../data/configs/";
		targetPath += _fileName;
		targetPath += ".plist";
		_internalFileName = targetPath;

		SL_PROCESS_APP()->log(ELogType_Info, "PersistentDictionary reading from %s", _internalFileName.c_str());
		CCDictionary* configDictionary(getRootDictionary()->createWithContentsOfFileThreadSafe(_internalFileName.c_str()));
		if(configDictionary == nullptr)	{
			setRootDictionary(CCDictionary::create());
		}
		else	{
			setRootDictionary(configDictionary);
		}
	}

	void PersistentDictionary::save()	{
		if(getRootDictionary() == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "PersistentDictionary failed saving - dictionary has not been set is empty");
			return;
		}

		// serialize the dictionary state
		if(_fileName.length() == 0)	{
			SL_PROCESS_APP()->log(ELogType_Error, "PersistentDictionary failed saving - filename is empty");
			return;
		}

		SLAString targetPath(PersistentDictionary::getWritablePath());
		targetPath += "../data/configs/";
		targetPath += _fileName;
		targetPath += ".plist";
		_internalFileName = targetPath;

		SL_PROCESS_APP()->log(ELogType_Info, "PersistentDictionary saving to %s", _internalFileName.c_str());
		getRootDictionary()->writeToFile(_internalFileName.c_str());
	}

	void PersistentDictionary::update( float dt )	{
		// nothing todo right now
		// the idea is auto serialization
		SLBaseClass::update(dt);
	}

	void PersistentDictionary::onEnter()
	{
		SLBaseClass::onEnter();
		scheduleUpdate();
	}

	void PersistentDictionary::onExit()
	{
		SLBaseClass::onExit();
	}

	const char* PersistentDictionary::getSerializeFileName() const
	{
		return _internalFileName.c_str();
	}

	CCDictionary* PersistentDictionary::getSubDictionary(const char* key)	{
		CCDictionary* rootDirectory(getRootDictionary());
		if(rootDirectory == nullptr)	{
			SL_PROCESS_APP()->log(ELogType_Error, "PersistentDictionary has no rootDirectory to retrieve a sub directory from");
			return nullptr;
		}
		CCDictionary* subDictionary(dynamic_cast<CCDictionary*>(rootDirectory->objectForKey(key)));
		if(subDictionary == nullptr)	{
			subDictionary = CCDictionary::create();
			rootDirectory->setObject(subDictionary,key);
		}
		return subDictionary;
	}

	void PersistentDictionary::dumpInfo(void) const
	{
		if(getRootDictionary() == nullptr)	{
			return;
		}
		// Dump
		CCPrettyPrinter visitor(0);
		getRootDictionary()->acceptVisitor(visitor);

		SL_PROCESS_APP()->log(ELogType_Info, "\n%s", visitor.getResult().c_str());


		// And Dump some warnings as well
#if CC_ENABLE_PROFILERS
		SL_PROCESS_APP()->log(ELogType_Info, "cocos2d: **** WARNING **** CC_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from ccConfig.h)");
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
		SL_PROCESS_APP()->log(ELogType_Info, "cocos2d: **** WARNING **** CC_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from ccConfig.h)");
#endif

	}

	CCDictionary* PersistentDictionary::getRootDictionary() const
	{
		if(SLBaseClass::getRootDictionary() == nullptr)	{
			AbstractPersistentDictionary* thisPtr(const_cast<PersistentDictionary*>(this));
			thisPtr->setRootDictionary(CCDictionary::create());
		}
		return SLBaseClass::getRootDictionary();
	}



}
