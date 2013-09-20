//!---------------------------------------------------------------------------
//! \file "slCCInt64.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#ifndef __CCINT64_H__
#define __CCINT64_H__

#if 0
#include "CCObject.h"
#endif
NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class slCocosUtil_API CCInt64 : public CCObject
//class CC_DLL CCInt64 : public CCObject
{
public:
    CCInt64(SLInt64 v)
        : m_nValue(v) {}
    SLInt64 getValue() const {return m_nValue;}

    static CCInt64* create(SLInt64 v)
    {
        CCInt64* pRet = new CCInt64(v);
        pRet->autorelease();
        return pRet;
    }
private:
    SLInt64 m_nValue;
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* __CCINT64_H__ */
