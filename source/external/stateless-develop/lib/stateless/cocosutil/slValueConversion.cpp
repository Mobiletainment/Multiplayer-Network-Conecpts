//!---------------------------------------------------------------------------
//! \file "slValueConversion.cpp"
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
#include "slValueConversion.h"
#include "slCCInt64.h"

namespace sl	{

	cocos2d::CCString* ValueConversion::getStringForObject( cocos2d::CCObject* object )
	{
		if(object != nullptr)	{
			CCString* sValue = dynamic_cast<CCString*>(object);
			if(sValue != nullptr)	{
				return sValue;
			}
			else	{
				CCInteger* iValue = dynamic_cast<CCInteger*>(object);
				if(iValue != nullptr)	{
					CCString* numValue = CCString::createWithFormat("%d", iValue->getValue());
					return numValue;
				}
				else	{
					CCFloat* fValue = dynamic_cast<CCFloat*>(object);
					if(fValue != nullptr)	{
						CCString* numValue = CCString::createWithFormat("%f", fValue->getValue());
						return numValue;
					}
					else	{
						CCDouble* dValue = dynamic_cast<CCDouble*>(object);
						if(dValue != nullptr)	{
							CCString* numValue = CCString::createWithFormat("%f", dValue->getValue());
							return numValue;
						}
						else	{
							CCBool* bValue = dynamic_cast<CCBool*>(object);
							if(bValue != nullptr)	{
								CCString* numValue = CCString::createWithFormat("%s", bValue->getValue() ? "true" : "false");
								return numValue;
							}
							else	{
								CCInt64* i64Value = dynamic_cast<CCInt64*>(object);
								if(i64Value != nullptr)	{
									CCString* numValue = CCString::createWithFormat("%lld", i64Value->getValue());
									return numValue;
								}
								else	{
								}
							}
						}
					}
				}
			}
		}
		return CCString::create("");
	}

	cocos2d::CCObject* ValueConversion::setValueFromString( cocos2d::CCString* strValue, cocos2d::CCObject* object )
	{
		// TODO: return the object itself if it has the same value
		CCString* sValue = dynamic_cast<CCString*>(object);
		if(sValue != nullptr)	{
			return strValue;
		}
		else	{
			CCInteger* iValue = dynamic_cast<CCInteger*>(object);
			if(iValue != nullptr)	{
				return CCInteger::create(strValue->intValue());
			}
			else	{
				CCFloat* fValue = dynamic_cast<CCFloat*>(object);
				if(fValue != nullptr)	{
					return CCFloat::create(strValue->floatValue());
				}
				else	{
					CCDouble* dValue = dynamic_cast<CCDouble*>(object);
					if(dValue != nullptr)	{
						return CCDouble::create(strValue->doubleValue());
					}
					else	{
						CCBool* bValue = dynamic_cast<CCBool*>(object);
						if(bValue != nullptr)	{
							return CCBool::create(strValue->boolValue());
						}
						else	{
							CCInt64* i64Value = dynamic_cast<CCInt64*>(object);
							if(i64Value != nullptr)	{
								SLInt64 valueInt64 = _atoi64(strValue->getCString());
								return CCInt64::create(valueInt64);
							}
							else	{
							}
						}
					}
				}
			}
		}
		return nullptr;
	}
}
