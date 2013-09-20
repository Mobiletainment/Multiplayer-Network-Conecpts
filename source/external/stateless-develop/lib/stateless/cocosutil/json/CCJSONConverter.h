//
//  CCJSONConverter.h
//  cocos2d-x-jc
//
//  Created by LIN BOYU on 11/14/12.
//
//  Adopted by Cyrus Preuss on 08/29/13.
//

#ifndef __cocos2d_x_jc__CCJSONConverter__
#define __cocos2d_x_jc__CCJSONConverter__

#define CCJC_CAST_CCNUMBER(__pDict__,__sKey__) dynamic_cast<CCNumber *>(__pDict__->objectForKey(__sKey__))
#define CCJC_CAST_CCSTRING(__pDict__,__sKey__) dynamic_cast<CCString *>(__pDict__->objectForKey(__sKey__))
#define CCJC_CAST_CCBOOL(__pDict__,__sKey__) dynamic_cast<CCBool *>(__pDict__->objectForKey(__sKey__))
#define CCJC_CAST_CCARRAY(__pDict__,__sKey__) dynamic_cast<CCArray *>(__pDict__->objectForKey(__sKey__))
#define CCJC_CAST_CCDICTIONARY(__pDict__,__sKey__) dynamic_cast<CCDictionary *>(__pDict__->objectForKey(__sKey__))
#define CCJC_CAST_CCNULL(__pDict__,__sKey__) dynamic_cast<CCNull *>(__pDict__->objectForKey(__sKey__))

namespace sl	{
	class slCocosUtil_API CCJSONConverter : public cocos2d::CCObject {
	public:
		static cocos2d::CCString* strFrom(cocos2d::CCDictionary * dictionary);    
		static cocos2d::CCDictionary * dictionaryFrom(const char * str);
	private:
	};
}

#endif /* defined(__cocos2d_x_jc__CCJSONConverter__) */