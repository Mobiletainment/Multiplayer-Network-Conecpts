//!---------------------------------------------------------------------------
//! \file "slPluginLauncher.cpp"
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
#include "slPluginLauncher.h"
#include "slCocosAppDelegate.h"


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


namespace sl	{


	SLAString getSharedLibSuffix()	{
		SLAString libSuffix(Poco::SharedLibrary::suffix());
#if defined(_DEBUG)
		libSuffix = replaceString(libSuffix, "d.", "_Debug.");
#else
		libSuffix = replaceString(libSuffix, ".", "_Release.");
#endif
		return libSuffix;
	}


	PluginLauncherPtr PluginLauncher::create()
	{
		return PluginLauncherPtr(new PluginLauncher());
	}


	PluginLauncher::PluginLauncher():_libName("")
	{

	}

	nlFunc_launch* PluginLauncher::queryLaunchFunction()
	{
		try
		{
			void* symbolPtr (_sharedLib.getSymbol("launch"));
			if(symbolPtr != nullptr)	{
				nlFunc_launch* launchFunc = (nlFunc_launch*)symbolPtr;
				SL_PROCESS_APP()->log( ELogType_Info, "found launch function lib: %s", _libName.c_str());
				return launchFunc;
			}
		}
		catch ( ... )
		{
		}
		return nullptr;
	}

	nlFunc_launchLayer* PluginLauncher::queryLaunchLayerFunction()
	{
		try
		{
			void* symbolPtr (_sharedLib.getSymbol("launchLayer"));
			if(symbolPtr != nullptr)	{
				nlFunc_launchLayer* launchLayerFunc = (nlFunc_launchLayer*)symbolPtr;
				SL_PROCESS_APP()->log( ELogType_Info, "found launchLayer function lib: %s", _libName.c_str());
				return launchLayerFunc;
			}
		}
		catch ( ... )
		{
		}
		return nullptr;
	}


	bool PluginLauncher::launch( Poco::Util::Application& app )
	{
		bool launched(false);
		if(_libName.length() > 0)	{
			try
			{
				_sharedLib.unload();

				Poco::Util::LayeredConfiguration& appConfig(app.config());
				SLAString appDir(appConfig.getString("application.dir"));

				SLAString libPath(appDir);
				libPath += _libName;
				libPath += sl::getSharedLibSuffix();
				_sharedLib.load(libPath);

				nlFunc_launchLayer* launchLayerFunc = queryLaunchLayerFunction();
				nlFunc_launch* launchFunc = queryLaunchFunction();
				if(launchFunc /*&& !launchLayerFunc*/)	{
					launchFunc();
					launched = true;
				}
				else	{
					if(launchLayerFunc)	{
						int returnCode(sl::CocosAppDelegate::launch(launchLayerFunc));
						launched = (returnCode == 0);
					}
					else	{
						SL_PROCESS_APP()->log( ELogType_Error, "failed to queryLaunchFunction lib: %s", _libName.c_str());
						SL_PROCESS_APP()->log( ELogType_Error, "failed to queryLaunchLayerFunction lib: %s", _libName.c_str());
					}
				}
				_sharedLib.unload();
			}
			catch ( ... )
			{
				app.logger().error("failed to launch lib: %s", _libName.c_str());
			}
		}
		return launched;
	}

	PluginLauncher::~PluginLauncher()
	{
		_sharedLib.unload();
	}

	void PluginLauncher::setLibName( const SLAString& libName )
	{
		_libName = libName;
	}

	bool PluginLauncher::hasLibName() const
	{
		return (_libName.length() > 0);
	}



	const char* Plugin::getModuleName() const
	{
		return _moduleName.c_str();
	}

	Plugin::Plugin(void* handle)
	{
#ifdef WIN32
		TCHAR szFileName[MAX_PATH];

		if(GetModuleFileNameA((HMODULE)handle, szFileName, MAX_PATH ) != 0)	{
			_moduleFullName = szFileName;
			Poco::Path path(_moduleFullName);
			_moduleName = path.getFileName();
		}
#endif
	}

}


