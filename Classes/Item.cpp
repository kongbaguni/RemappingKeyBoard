//
//  Item.cpp
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#include "Item.h"
Item::Item():
_pLabel(nullptr)
{
    
}

Item::~Item()
{
    CC_SAFE_RELEASE_NULL(_pLabel);
    
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
    
    
    auto label = Label::createWithBMFont("fonts/bmfont.fnt",strValue);
    label->setPosition(getContentSize()*0.5f);

    addChild(label);
    setLabel(label);
    colorChange();
    return true;
}

std::string Item::getStringValue()
{
    return _pLabel->getString();
}

void Item::colorChange()
{
    auto strValue = _pLabel->getString();
    _pLabel->setScale(1.6f/strValue.size()+0.2f);
    _pLabel->setColor(Color3B::BLUE);
    if (strValue.compare("KC_TRNS")==0)
    {
        _pLabel->setColor(Color3B::WHITE);
    }
    else if (strValue.compare("FUNC(2)")==0 || strValue.compare("FUNC(1)")==0)
    {
        _pLabel->setColor(Color3B::RED);
    }
    else if (strValue.size()<=4)
    {
        _pLabel->setColor(Color3B::YELLOW);
    }
}

void Item::setStringValueWithColorChange(std::string strValue)
{
    _pLabel->setString(strValue);
    colorChange();
}