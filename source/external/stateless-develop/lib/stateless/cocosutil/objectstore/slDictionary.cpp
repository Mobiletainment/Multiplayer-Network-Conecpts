//!---------------------------------------------------------------------------
//! \file "slDictionary.cpp"
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
#include "slDictionary.h"
#include "json/CCJSONConverter.h"

namespace sl	{


	Dictionary::Dictionary()
	{

	}

	Dictionary::~Dictionary()
	{

	}

	// TODO @CP : this is a very inefficient hack ...
	CCDictionary* Dictionary::copyDictionary( CCDictionary* source )
	{
		// see: not implemented
		/*
		CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict)
		{
		CCDictionary* pNewDict = (CCDictionary*)srcDict->copy();
		pNewDict->autorelease();
		return pNewDict;
		}
		*/
		return CCJSONConverter::dictionaryFrom(CCJSONConverter::strFrom(source)->getCString());
	}

	void Dictionary::setObjectIfNotExist( CCDictionary* target, CCObject* pObject, intptr_t key )
	{
		CCObject* existingObject(target->objectForKey(key));
		if(existingObject == nullptr)	{
			target->setObject(pObject, key);
		}
	}

	void Dictionary::setObjectIfNotExist( CCDictionary* target, CCObject* pObject, const char* key )
	{
		CCObject* existingObject(target->objectForKey(key));
		if(existingObject == nullptr)	{
			target->setObject(pObject, key);
		}
	}

	void Dictionary::setObjectIfNotExist( CCDictionary* target, const char* value, const char* key )
	{
		CCObject* existingObject(target->objectForKey(key));
		if(existingObject == nullptr)	{
			target->setObject(CCString::create(value), key);
		}
	}


	const char* Dictionary::getString( CCDictionary* dictionary, const char* key, const char* defaultValue /*= "" */ )
	{
		CCString* dictValue(dynamic_cast<CCString*>(dictionary->objectForKey(key)));
		if(dictValue)	{
			return dictValue->getCString();
		}
		return defaultValue;
	}

	void Dictionary::setString( CCDictionary* dictionary, const char* key, const char* value )
	{
		dictionary->setObject(CCString::create(value),key);
	}

	bool Dictionary::getBool( CCDictionary* dictionary, const char* key, bool defaultValue /*= false */ )
	{
		CCBool* dictValue(dynamic_cast<CCBool*>(dictionary->objectForKey(key)));
		if(dictValue)	{
			return dictValue->getValue();
		}
		return defaultValue;
	}

	void Dictionary::setBool( CCDictionary* dictionary, const char* key, bool value )
	{
		dictionary->setObject(CCBool::create(value),key);
	}

	int Dictionary::getInteger( CCDictionary* dictionary, const char* key, int defaultValue /*= 0 */ )
	{
		CCInteger* dictValue(dynamic_cast<CCInteger*>(dictionary->objectForKey(key)));
		if(dictValue)	{
			return dictValue->getValue();
		}
		return defaultValue;
	}

	void Dictionary::setInteger( CCDictionary* dictionary, const char* key, int value )
	{
		dictionary->setObject(CCInteger::create(value),key);
	}

	float Dictionary::getFloat( CCDictionary* dictionary, const char* key, float defaultValue /*= 0 */ )
	{
		CCObject* o(dictionary->objectForKey(key));
		if(o != nullptr)	{
			CCFloat* dictValue(dynamic_cast<CCFloat*>(o));
			if(dictValue)	{
				return dictValue->getValue();
			}
			else	{
				CCDouble* dictValued(dynamic_cast<CCDouble*>(o));
				if(dictValued)	{
					return (float)dictValued->getValue();
				}
				else	{
					CCInteger* dictValuei(dynamic_cast<CCInteger*>(o));
					if(dictValuei)	{
						return (float)dictValuei->getValue();
					}
				}
			}
		}
		return defaultValue;
	}

	void Dictionary::setFloat( CCDictionary* dictionary, const char* key, float value )
	{
		dictionary->setObject(CCFloat::create(value),key);
	}

	double Dictionary::getDouble( CCDictionary* dictionary, const char* key, double defaultValue /*= 0 */ )
	{
		CCObject* o(dictionary->objectForKey(key));
		if(o != nullptr)	{
			CCDouble* dictValue(dynamic_cast<CCDouble*>(o));
			if(dictValue)	{
				return dictValue->getValue();
			}
			else	{
				float f(Dictionary::getFloat(dictionary, key, static_cast<float>(defaultValue)));
				return static_cast<double>(f);
			}
		}
		return defaultValue;
	}

	void Dictionary::setDouble( CCDictionary* dictionary, const char* key, double value )
	{
		dictionary->setObject(CCDouble::create(value),key);
	}

	CCDictionary* Dictionary::getDictionary( CCDictionary* dictionary, const char* key )
	{
		if(dictionary != nullptr)	{
			return dynamic_cast<CCDictionary*>(dictionary->objectForKey(key));
		}
		return nullptr;
	}


}
