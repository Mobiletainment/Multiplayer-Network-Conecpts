#pragma once
#ifndef __SLCOCOSGAME_H__
#define __SLCOCOSGAME_H__

//!---------------------------------------------------------------------------
//! \file "slCocosGame.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------



#include "slCocosGameAPI.h"

#include "slCocosGameMacros.h"
#include "slCocosGameEnums.h"
#include "slCocosGameTypes.h"

//-----------------------------------------------------------------------------

// users of this library have to include slSimulation
// so to prevent them from any include problems
// we include the header here
#include <stateless/simulation/slSimulation.h>

#include "actor/slGameActor.h"
#include "actor/slGameActorNode.h"
#include "actor/slGameActorComponent.h"
#include "actor/slActorSprite.h"

#include "network/slGameObjectReplica.h"
#include "network/slReplicaComponent.h"
#include "network/slPeerObserverNode.h"

#include "vehicle/slPedestrian.h"

#include "slGameplayLayer.h"
#include "slPedestrianPath.h"

#endif  // __SLCOCOSGAME_H__
