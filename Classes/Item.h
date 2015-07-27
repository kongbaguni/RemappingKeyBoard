//
//  Item.h
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#ifndef __Remapping__Item__
#define __Remapping__Item__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class Item : public Sprite
{
    
public:
    static Item* create(std::string strValue);
protected:
    Item();
    virtual ~Item();
    virtual bool init(std::string strValue);
    CC_SYNTHESIZE_READONLY(std::string, _stringValue, StringValue);
};
#endif /* defined(__Remapping__Item__) */
