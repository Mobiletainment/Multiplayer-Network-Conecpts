//!---------------------------------------------------------------------------
//! \file "nlLauncherApplication.cpp"
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
#include "nlLauncherApplication.h"

#include <iostream>
#include <sstream>
#include <iostream>
#include <sstream>

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;

#if 0
// poco provides a macro here but it is not usefull in our context
POCO_APP_MAIN(SampleApp)
#endif

namespace nl	{

	bool LauncherApplication::initFromWinCommandline()
	{
		bool initialized(false);
#ifdef WIN32
		int argCount;
		//		wchar_t** szArgList(CommandLineToArgvW(GetCommandLineW(), &argCount));
		LPWSTR* szArgList(CommandLineToArgvW(GetCommandLineW(), &argCount));

		try
		{
			init(argCount, szArgList);
			initialized = true;
		}
		catch (...)
		{
			std::cout << "ERROR initFromWinCommandline" << std::endl;			
		}

		LocalFree(szArgList);
#endif
		return initialized;
	}

	LauncherApplication::LauncherApplication(): _helpRequested(false)
	{
		_pluginLauncher = PluginLauncher::create();
	}

	void LauncherApplication::initialize( Poco::Util::Application& self )
	{
		loadConfiguration(); // load default configuration files, if present
		SLBaseClass::initialize(self);
		// add your own initialization code here

	}

	void LauncherApplication::uninitialize()
	{
		// add your own uninitialization code here
		SLBaseClass::uninitialize();

	}

	void LauncherApplication::reinitialize( Poco::Util::Application& self )
	{
		SLBaseClass::reinitialize(self);
		// add your own reinitialization code here

	}

	void LauncherApplication::defineOptions( Poco::Util::OptionSet& options )
	{
		SLBaseClass::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<LauncherApplication>(this, &LauncherApplication::handleHelp)));

		options.addOption(
			Option("lib", "l", "load the specified lib and call it's main entry function")
			.required(false)
			.repeatable(false)
			.argument("fileName")
			.callback(OptionCallback<LauncherApplication>(this, &LauncherApplication::handleLib)));

		options.addOption(
			Option("clients", "c", "start client peers only")
			.required(false)
			.repeatable(false)
			.argument("fileName")
			.callback(OptionCallback<LauncherApplication>(this, &LauncherApplication::handleClientsOnly)));

/*
		options.addOption(
			Option("define", "D", "define a configuration property")
			.required(false)
			.repeatable(true)
			.argument("name=value")
			.callback(OptionCallback<LauncherApplication>(this, &LauncherApplication::handleDefine)));

		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
			.required(false)
			.repeatable(true)
			.argument("file")
			.callback(OptionCallback<LauncherApplication>(this, &LauncherApplication::handleConfig)));

		options.addOption(
			Option("bind", "b", "bind option value to test.property")
			.required(false)
			.repeatable(false)
			.argument("value")
			.binding("test.property"));
*/
	}


	void LauncherApplication::handleOption( const std::string& name, const std::string& value )
	{
		// base class calls option callback
		SLBaseClass::handleOption(name,value);
	}

	void LauncherApplication::handleHelp( const std::string& name, const std::string& value )
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();

	}

	void LauncherApplication::handleDefine( const std::string& name, const std::string& value )
	{
		defineProperty(value);

	}

	void LauncherApplication::handleConfig( const std::string& name, const std::string& value )
	{
		loadConfiguration(value);

	}

	void LauncherApplication::displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
		helpFormatter.format(std::cout);

	}

	void LauncherApplication::defineProperty( const std::string& def )
	{
		std::string name;
		std::string value;
		std::string::size_type pos = def.find('=');
		if (pos != std::string::npos)
		{
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		config().setString(name, value);

	}


	void LauncherApplication::handleLib( const std::string& name, const std::string& value )
	{
		_mainLibName = value;
		_pluginLauncher->setLibName(value);
	}

	void LauncherApplication::handleClientsOnly( const std::string& name, const std::string& value )
	{
		
	}


	int LauncherApplication::main( const ArgVec& args )
	{
		int appExitCode(Application::EXIT_OK);
		if (!_helpRequested)
		{
			if(_pluginLauncher->hasLibName())	{
				if(_pluginLauncher->launch(*this))	{

				}
				else	{
					appExitCode = Application::EXIT_SOFTWARE;
				}
			}
			else	{
				log(ELogType_Info,"Command line:");
				std::ostringstream ostr;
				for (ArgVec::const_iterator it = argv().begin(); it != argv().end(); ++it)
				{
					ostr << *it << ' ';
				}
				log(ELogType_Info,ostr.str());
				log(ELogType_Info,"Arguments to main():");
				for (ArgVec::const_iterator it = args.begin(); it != args.end(); ++it)
				{
					log(ELogType_Info,*it);
				}
				log(ELogType_Info,"Application properties:");
				printProperties("");
			}
		}		
		return appExitCode;
	}

	void LauncherApplication::printProperties( const std::string& base )
	{
		AbstractConfiguration::Keys keys;
		config().keys(base, keys);
		if (keys.empty())
		{
			if (config().hasProperty(base))
			{
				std::string msg;
				msg.append(base);
				msg.append(" = ");
				msg.append(config().getString(base));
				log(ELogType_Info,msg);
			}
		}
		else
		{
			for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
			{
				std::string fullKey = base;
				if (!fullKey.empty()) fullKey += '.';
				fullKey.append(*it);
				printProperties(fullKey);
			}
		}
	}


}

