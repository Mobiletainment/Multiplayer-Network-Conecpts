#pragma once
#ifndef __SLPROCESSAPPLICATION_H__
#define __SLPROCESSAPPLICATION_H__
//!---------------------------------------------------------------------------
//! \file "slProcessApplication.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------




#ifndef POCO_NO_AUTOMATIC_LIBS
#  pragma message( "--------------------------" )
#  pragma message( " Poco dependency: POCO_NO_AUTOMATIC_LIBS has not been defined" )
#  pragma message( " defining it now" )
#  pragma message( "--------------------------" )
#  define POCO_NO_AUTOMATIC_LIBS
#else
#  pragma message( "--------------------------" )
#  pragma message( " Poco dependency: POCO_NO_AUTOMATIC_LIBS is defined" )
#  pragma message( "--------------------------" )
#endif

#include <Poco/Config.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>
#include <Poco/SharedLibrary.h>

#include "slApplicationMacros.h"
#include "slApplicationEnums.h"

namespace sl	{

	class ProcessApplication;

	typedef Poco::AutoPtr<ProcessApplication> ProcessApplicationPtr;

	class slApplication_API ProcessApplication: public Poco::Util::Application
	{
		SL_DECLARE_BASE(Poco::Util::Application);
	public:
		static ProcessApplication* sharedInstance();

		ProcessApplication();

		void log(ELogType type, const SLAString& format, ...);
		void log(ELogType type, const char* format, ...);

		void log(ELogType type, EConsoleBackColor backColor, const SLAString& format, ...);
		void log(ELogType type, EConsoleBackColor backColor, const char* format, ...);


	protected:	
		virtual ~ProcessApplication();


	private:
	};


}


#endif
