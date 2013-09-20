#pragma once
#ifndef __SLKEYBOARDINPUT_H__
#define __SLKEYBOARDINPUT_H__

//!---------------------------------------------------------------------------
//! \file "slKeyBoardInput.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


class slCocosUtil_API KeyBoardInput
{
public:
	static void setup(void);
	static bool isKeyDown( char key );
};

#endif  // __SLKEYBOARDINPUT_H__
