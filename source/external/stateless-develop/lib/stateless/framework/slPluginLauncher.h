//!---------------------------------------------------------------------------
//! \file "slPluginLauncher.h"
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
#ifndef __SLPLUGINLAUNCHER_H__
#define __SLPLUGINLAUNCHER_H__

#include <Poco/Config.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/SharedLibrary.h>

#include "slPluginInterface.h"

namespace sl	{
	class PluginLauncher;

	typedef Poco::AutoPtr<PluginLauncher> PluginLauncherPtr;

	class slFramework_API PluginLauncher : public Poco::RefCountedObject
	{
	public:
		static PluginLauncherPtr create();

		void setLibName(const SLAString& libName);
		bool hasLibName() const;

		bool launch(Poco::Util::Application& app);
	protected:
		PluginLauncher();
		virtual ~PluginLauncher();
	private:

		nlFunc_launch* queryLaunchFunction();
		nlFunc_launchLayer* queryLaunchLayerFunction();

		Poco::SharedLibrary _sharedLib;
		SLAString _libName;
	};

	class slFramework_API Plugin	{
	public:
		explicit Plugin(void* handle);
		const char* getModuleName() const;
	private:

#pragma warning (push)
#pragma warning(disable:4251) // needs to have dll-interface to be used by clients of class
		SLAString _moduleName;
		SLAString _moduleFullName;
#pragma warning (pop)
	};

}


#endif
