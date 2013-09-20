//!---------------------------------------------------------------------------
//! \file "slConsole.h"
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
#ifndef __SLCONSOLE_H__
#define __SLCONSOLE_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2013, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

#include "slApplicationEnums.h"

namespace sl	{

	class slApplication_API Console
	{
	public:
		static Console& sharedInstance();

		bool hasCreatedConsole() const;

		static void setLogColor( ELogType eType, EConsoleBackColor backColor );

		static char * getStringFromStdIn ( char * str, int num );

	private:
		Console();
		virtual ~Console();

		bool _hasCreatedConsole;
	};

	class slApplication_API ConsoleColorScope
	{
	public:
		ConsoleColorScope();
		ConsoleColorScope(ELogType type);
		virtual ~ConsoleColorScope();

	private:
		void init();
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBufferInfoStdOut;
		CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBufferInfoStdError;
#endif
	};

	class slApplication_API ConsoleBackColorScope
	{
	public:
		ConsoleBackColorScope();
		ConsoleBackColorScope(ELogType type, EConsoleBackColor backColor);
		virtual ~ConsoleBackColorScope();

	private:
		void init();
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBufferInfoStdOut;
		CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBufferInfoStdError;
#endif
	};
}


#endif
