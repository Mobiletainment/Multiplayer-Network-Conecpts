//!---------------------------------------------------------------------------
//! \file "slProcessApplication.cpp"
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
#include "slProcessApplication.h"
#include "slConsole.h"

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

namespace sl	{

	ProcessApplication* g_sharedInstance(NULL);

	ProcessApplication::ProcessApplication()	{
		if(g_sharedInstance == NULL)	{
			g_sharedInstance = this;
		}
	}

	ProcessApplication::~ProcessApplication()	{
		g_sharedInstance = NULL;
	}

	ProcessApplication* ProcessApplication::sharedInstance()
	{
		return g_sharedInstance;
	}

	void ProcessApplication::log(ELogType type, const SLAString& format, ...)	{
		CCString* tempString = new CCString();
		va_list ap;
		va_start(ap, format);
		bool initialized(tempString->initWithFormatAndValist(format.c_str(), ap));
		va_end(ap);
		if(initialized)	{
			log(type, EConsoleBackColor_Black, tempString->getCString());
		}
		tempString->release();
	}

	void ProcessApplication::log(ELogType type, const char* format, ...)	{
		CCString* tempString = new CCString();
		va_list ap;
		va_start(ap, format);
		bool initialized(tempString->initWithFormatAndValist(format, ap));
		va_end(ap);
		if(initialized)	{
			log(type, EConsoleBackColor_Black, tempString->getCString());
		}
		tempString->release();
	}

	void ProcessApplication::log( ELogType type, EConsoleBackColor backColor, const SLAString& format, ... )
	{
		CCString* tempString = new CCString();
		va_list ap;
		va_start(ap, format);
		bool initialized(tempString->initWithFormatAndValist(format.c_str(), ap));
		va_end(ap);
		if(initialized)	{
			log(type, backColor, tempString->getCString());
		}
		tempString->release();
	}

	void ProcessApplication::log( ELogType type, EConsoleBackColor backColor, const char* format, ... )
	{
		CCString* tempString = new CCString();
		va_list ap;
		va_start(ap, format);
		bool initialized(tempString->initWithFormatAndValist(format, ap));
		va_end(ap);
		if(initialized)	{
			ConsoleBackColorScope colorScope(type, backColor);
			const char* logString(tempString->getCString());
			switch(type)	{
			case( ELogType_Info ):
				{
					logger().information(logString);
				}
				break;
			case( ELogType_Status ):
				{
					logger().information(logString);
				}
				break;
			case( ELogType_Message ):
				{
					logger().information(logString);
				}
				break;
			case( ELogType_Warning ):
				{
					logger().information(logString);
				}
				break;
			case( ELogType_Error ):
				{
					logger().information(logString);
				}
				break;
			default:
				logger().information(logString);
				break;
			}

			// TODO: is debugger present ...
			WCHAR wszBuf[cocos2d::kMaxLogLen + 1] = {0};
			MultiByteToWideChar(CP_UTF8, 0, logString, -1, wszBuf, sizeof(wszBuf));
			OutputDebugStringW(wszBuf);
			OutputDebugStringA("\n");
		}
		tempString->release();
	}

}

