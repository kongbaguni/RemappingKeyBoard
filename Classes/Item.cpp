//
//  Item.cpp
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#include "Item.h"
Item::Item()
{
    
}

Item::~Item()
{
    
}

Item* Item::create(std::string strValue)
{
    auto item = new Item();
    if (item && item->init(strValue))
    {
        item->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(item);
    }
    return item;
}

bool Item::init(std::string strValue)
{
    if (!Sprite::initWithFile("res/box.png"))
    {
        return false;
    }
    
    _stringValue=strValue;
    
    auto label = Label::createWithBMFont("fonts/bmfont.fnt",strValue);
    label->setColor(Color3B::BLUE);
    label->setPosition(getContentSize()*0.5f);
    label->setScale(0.5f);
    addChild(label);
    
    return true;
}