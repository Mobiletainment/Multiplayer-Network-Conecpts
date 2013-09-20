//!---------------------------------------------------------------------------
//! \file "slCocosUtilMacros.h"
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
#ifndef __SLCOCOSUTILMACROS_H__
#define __SLCOCOSUTILMACROS_H__

#define NL_UI_CTRL_FONTNAME "Marker Felt"
#define NL_UI_CTRL_FONTSIZE 18


#define NL_UI_LBL_FONTNAME "Arial"
#define NL_UI_LBL_FONTSIZE 18
//#define NL_UI_LBL_COLOR ccc3(255, 255, 255)
#define NL_UI_LBL_COLOR ccc3(200, 200, 200)

#define NL_UI_TEXTFIELD_FONTNAME "Arial"
#define NL_UI_TEXTFIELD_FONTSIZE 18
#define NL_UI_TEXTFIELD_COLOR ccc3(255, 255, 255)



#define NL_APPLAYER_FRAME_WIDTH 0.0f

// control tags
#define SL_CTRLID_OK 1000
#define SL_CTRLID_CANCEL 1001


#define SL_CTRLID_INFO 1050
#define SL_CTRLID_OPTIONS 1051
#define SL_CTRLID_TASKS 1052

#define SL_CTRLID_QUADLAYER_RECREATE 1060
#define SL_CTRLID_QUADLAYER_RECREATE_TWINLAYER 1061
#define SL_CTRLID_QUADLAYER_RECREATE_TWO_TWINLAYERS 1062
#define SL_CTRLID_QUADLAYER_RECREATE_THREE_TWINLAYERS 1063
#define SL_CTRLID_QUADLAYER_RECREATE_04 1064

#define SL_CTRLID_QUADLAYER_RECREATE_SINGLEPLAYER 1065
#define SL_CTRLID_QUADLAYER_RECREATE_CLIENTSERVER 1066
#define SL_CTRLID_QUADLAYER_RECREATE_PEERTOPEER 1067




// notifications
#define NL_NOTIFY_KEY_DOWN "notify_keydown"
#define NL_NOTIFY_KEY_UP "notify_keyup"

#define SL_NOTIFY_CTRL_EDIT_END "ctrl_edit_end"

#endif // __SLCOCOSUTILMACROS_H__
