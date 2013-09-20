//!---------------------------------------------------------------------------
//! \file "nlMain.cpp"
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
#include <conio.h>
#include <iostream>

#include "nlLauncherApplication.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	nl::Console& console(nl::Console::sharedInstance());
	int exitCode(Poco::Util::Application::EXIT_SOFTWARE);

	bool repeat(true);

	while(repeat == true)	{
		repeat = false;
		// note: application scope
		{
			try
			{
				nl::LauncherApplicationPtr app(new nl::LauncherApplication());
				if(	app->initFromWinCommandline() )	{
					exitCode = app->run();
				}
				app->log(nl::ELogType_Info, "press q to quit");
				app->log(nl::ELogType_Info, "press r to repeat");
			}
			catch (...)
			{
#if _DEBUG
				std::cout << "ERROR running main application" << std::endl;			
#else
				std::cout << "ERROR running main application" << std::endl;			
#endif
			}
		}

		if(console.hasCreatedConsole())	{
			int consoleChar(getch());
			char inputChar(static_cast<char>(consoleChar));
			if(inputChar == 'r')	{
				repeat = true;
			}
			else	{
				while(inputChar != 'q')	{
					consoleChar = getch();
					inputChar = static_cast<char>(consoleChar);
					if(inputChar == 'r')	{
						repeat = true;
						break;
					}
				}
			}
		}
	}

	return exitCode;
}

