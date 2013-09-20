//!---------------------------------------------------------------------------
//! \file "nlLauncherApplication.h"
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
#ifndef __NLNETAPPLICATION_H__
#define __NLNETAPPLICATION_H__

#include <Poco/Config.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>

#include <stateless/application/slApplication.h>
#include <stateless/framework/slFramework.h>

namespace nl	{
	using namespace sl;
	/// This implementation demonstrates some of the features of the Util::Application class,
	/// such as configuration file handling and command line arguments processing.
	///
	/// Try LauncherApplication --help (on Unix platforms) or LauncherApplication /help (elsewhere) for
	/// more information.
	class LauncherApplication: public ProcessApplication
	{
		SL_DECLARE_BASE(ProcessApplication);
	public:
		LauncherApplication();

		bool initFromWinCommandline();

	protected:	
//		virtual const char* name() const; // TODO
		virtual void initialize(Poco::Util::Application& self);
		virtual void uninitialize();
		virtual void reinitialize(Poco::Util::Application& self);
		virtual void defineOptions(Poco::Util::OptionSet& options);

		virtual void handleOption(const std::string& name, const std::string& value);


		virtual int main(const ArgVec& args);

	private:
		void handleLib(const std::string& name, const std::string& value);
		void handleHelp(const std::string& name, const std::string& value);
		void handleDefine(const std::string& name, const std::string& value);
		void handleConfig(const std::string& name, const std::string& value);
		void handleClientsOnly(const std::string& name, const std::string& value);

		void displayHelp();
		void defineProperty(const std::string& def);
		void printProperties(const std::string& base);

		PluginLauncherPtr _pluginLauncher;
		SLAString _mainLibName;
		bool _helpRequested;
	};

	SL_POCO_PTR(LauncherApplication)


}


#endif
