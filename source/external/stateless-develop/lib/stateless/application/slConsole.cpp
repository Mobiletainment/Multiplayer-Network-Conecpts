//!---------------------------------------------------------------------------
//! \file "slConsole.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2013, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

#include "stdafx.h"
#include "slConsole.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>

namespace sl	{


	Console& Console::sharedInstance()
	{
		static Console console;
		return console;
	}

	Console::Console():_hasCreatedConsole(false)	{
#ifdef WIN32

#if 0
		if( _isatty( _fileno( stdin ) ) ) {
			/* this is a terminal */
		}
#endif
		HWND consoleWnd = GetConsoleWindow();
		DWORD dwConsoleProcessId(0);
		DWORD dwCurrentProcessId(GetCurrentProcessId());
		if(consoleWnd!=NULL)	{
			GetWindowThreadProcessId(consoleWnd, &dwConsoleProcessId);
		}
		if (dwCurrentProcessId==dwConsoleProcessId)
		{
		}
		else
		{
			AllocConsole();

			int hConHandle;
			intptr_t lStdHandle;
			//	CONSOLE_SCREEN_BUFFER_INFO coninfo;
			FILE *fp;
			// redirect unbuffered STDOUT to the console
			lStdHandle = (intptr_t)GetStdHandle(STD_OUTPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen( hConHandle, "w" );
			*stdout = *fp;
			setvbuf( stdout, NULL, _IONBF, 0 );

			// redirect unbuffered STDIN to the console
			lStdHandle = (intptr_t)GetStdHandle(STD_INPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen( hConHandle, "r" );
			*stdin = *fp;
			setvbuf( stdin, NULL, _IONBF, 0 );

			// redirect unbuffered STDERR to the console
			lStdHandle = (intptr_t)GetStdHandle(STD_ERROR_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen( hConHandle, "w" );
			*stderr = *fp;
			setvbuf( stderr, NULL, _IONBF, 0 );

			_hasCreatedConsole = true;
		}
#endif
		// makes cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
		std::ios::sync_with_stdio(); 

#if 0
		ConsoleColorScope colorScope(ELogType_Info);
		std::cout << "consoleWnd: " << consoleWnd << std::endl;
		std::cout << "  dwProcessId: " << dwConsoleProcessId << std::endl;
		std::cout << "dwCurrentProcessId: " << dwCurrentProcessId << std::endl;
#endif
	}

	Console::~Console()	{
#ifdef WIN32
		if(this->_hasCreatedConsole)	{
			FreeConsole();
		}
#endif
	}

	bool Console::hasCreatedConsole() const
	{
		return this->_hasCreatedConsole;
	}

	char* Console::getStringFromStdIn ( char * str, int num )
	{
		fgets(str, num, stdin);
		if (str[0]=='\n' || str[0]=='\r')
			str[0]=0;

		size_t len=strlen(str);
		if (len>0 && (str[len-1]=='\n' || str[len-1]=='\r'))
			str[len-1]=0;
		if (len>1 && (str[len-2]=='\n' || str[len-2]=='\r'))
			str[len-2]=0;

		return str;
	}

	//-----------------------------------------------------------------------------
	void Console::setLogColor( ELogType eType, EConsoleBackColor backColor )
	{
#ifdef WIN32
		HANDLE hOutputStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hOutputStdError = ::GetStdHandle(STD_ERROR_HANDLE);
		if(( hOutputStdOut != NULL ) && ( hOutputStdError != NULL ))
		{
//			WORD dwBackGround(BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY);
			WORD dwBackGround(0);
			switch(backColor)	{
			case(EConsoleBackColor_Grey):
				dwBackGround = BACKGROUND_INTENSITY;
				break;
			default:
				dwBackGround = 0;
			}

			switch( eType )
			{
			case( ELogType_Info ):
				{
					::SetConsoleTextAttribute( hOutputStdOut, dwBackGround|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY );
					::SetConsoleTextAttribute( hOutputStdError, dwBackGround|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY );
				}
				break;
			case( ELogType_Status ):
				{
					::SetConsoleTextAttribute( hOutputStdOut, dwBackGround|FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN );
					::SetConsoleTextAttribute( hOutputStdError, dwBackGround|FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN );
				}
				break;
			case( ELogType_Message ):
				{
					::SetConsoleTextAttribute( hOutputStdOut, dwBackGround|FOREGROUND_GREEN|FOREGROUND_INTENSITY );
					::SetConsoleTextAttribute( hOutputStdError, dwBackGround|FOREGROUND_GREEN|FOREGROUND_INTENSITY );
				}
				break;
			case( ELogType_Warning ):
				{
					::SetConsoleTextAttribute( hOutputStdOut, dwBackGround|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY );
					::SetConsoleTextAttribute( hOutputStdError, dwBackGround|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY );
				}
				break;
			case( ELogType_Error ):
				{
					::SetConsoleTextAttribute( hOutputStdOut, dwBackGround|FOREGROUND_RED|FOREGROUND_INTENSITY );
					::SetConsoleTextAttribute( hOutputStdError, dwBackGround|FOREGROUND_RED|FOREGROUND_INTENSITY );
				}
				break;
			}
		}
#endif
	}



	ConsoleColorScope::ConsoleColorScope()
	{
		ConsoleColorScope::init();
	}

	ConsoleColorScope::ConsoleColorScope( ELogType type )
	{
		ConsoleColorScope::init();
		Console::setLogColor(type, EConsoleBackColor_Black);
	}

	ConsoleColorScope::~ConsoleColorScope()
	{
#ifdef WIN32
		HANDLE hConsoleOutput;
		hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		::SetConsoleTextAttribute( hConsoleOutput, _consoleScreenBufferInfoStdOut.wAttributes );
		hConsoleOutput = ::GetStdHandle(STD_ERROR_HANDLE);
		::SetConsoleTextAttribute( hConsoleOutput, _consoleScreenBufferInfoStdOut.wAttributes );
#endif
	}

	void ConsoleColorScope::init()
	{
#ifdef WIN32
		HANDLE hConsoleOutput;
		hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
		if( GetConsoleScreenBufferInfo(hConsoleOutput, &consoleScreenBufferInfo) == TRUE )	{
			_consoleScreenBufferInfoStdOut = consoleScreenBufferInfo;
		}
		hConsoleOutput = ::GetStdHandle(STD_ERROR_HANDLE);
		if( GetConsoleScreenBufferInfo(hConsoleOutput, &consoleScreenBufferInfo) == TRUE )	{
			_consoleScreenBufferInfoStdError = consoleScreenBufferInfo;
		}
#endif
	}


	ConsoleBackColorScope::ConsoleBackColorScope()
	{
		ConsoleBackColorScope::init();
	}

	ConsoleBackColorScope::ConsoleBackColorScope( ELogType type, EConsoleBackColor backColor )
	{
		ConsoleBackColorScope::init();
		Console::setLogColor(type, backColor);
	}

	ConsoleBackColorScope::~ConsoleBackColorScope()
	{
#ifdef WIN32
		HANDLE hConsoleOutput;
		hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		::SetConsoleTextAttribute( hConsoleOutput, _consoleScreenBufferInfoStdOut.wAttributes );
		hConsoleOutput = ::GetStdHandle(STD_ERROR_HANDLE);
		::SetConsoleTextAttribute( hConsoleOutput, _consoleScreenBufferInfoStdOut.wAttributes );
#endif
	}

	void ConsoleBackColorScope::init()
	{
#ifdef WIN32
		HANDLE hConsoleOutput;
		hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
		if( GetConsoleScreenBufferInfo(hConsoleOutput, &consoleScreenBufferInfo) == TRUE )	{
			_consoleScreenBufferInfoStdOut = consoleScreenBufferInfo;
		}
		hConsoleOutput = ::GetStdHandle(STD_ERROR_HANDLE);
		if( GetConsoleScreenBufferInfo(hConsoleOutput, &consoleScreenBufferInfo) == TRUE )	{
			_consoleScreenBufferInfoStdError = consoleScreenBufferInfo;
		}
#endif
	}

}

