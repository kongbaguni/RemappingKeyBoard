//
//  Board.cpp
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#include "Board.h"
#include <iostream>
#include <fstream>
#include <json/document.h>

Board::Board():
_pSelectItem(nullptr),
_pShadowItem(nullptr),
_pMenu(nullptr),
_eMode(mode::MOVE)
{
    _posList.clear();
    for (int k = 3; k >= 0; k--)
    {
        for (int i=4 ; i>0; i--)
        {
            float y = i*16 + k*16*5;
            for (int j=1; j<13; j++)
            {
                float x = j*32;
                _posList.push_back(Vec2(x, y));
            }
        }
    }
    
    for (int i=1; i>=0; i--)
    {
        float x = 420+32*i;
        for (int j=15; j>=0; j--)
        {
            float y = j*16+ 16;
            _posList.push_back(Vec2(x, y));
            
        }
    }
    _stringList.clear();
    
}
Board::~Board()
{
    CC_SAFE_RELEASE_NULL(_pShadowItem);
    CC_SAFE_RELEASE_NULL(_pSelectItem);
    CC_SAFE_RELEASE_NULL(_pMenu);
    
}

bool Board::init()
{
    if (!LayerColor::initWithColor(Color4B::WHITE))
    {
        return false;
    }
    
    //터치 이벤트 등록
    {
        auto listener =
        EventListenerTouchAllAtOnce::create();
        
        listener->onTouchesBegan = CC_CALLBACK_2(Board::onTouchesBegan, this);
        listener->onTouchesCancelled = CC_CALLBACK_2(Board::onTouchesCancelled, this);
        listener->onTouchesMoved = CC_CALLBACK_2(Board::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(Board::onTouchesEnded, this);

        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    


    return true;
}

void Board::onEnter()
{
    LayerColor::onEnter();
    showMenu();
}
void Board::sortItems()
{
}

void Board::showMenu()
{
    if (_pMenu==nullptr && getParent())
    {
        //메뉴 아이템 초기화
        {
            auto callBack = CC_CALLBACK_1(Board::menuCallBack, this);
            auto resetBtn =
            MenuItemLabel::create(Label::createWithBMFont("fonts/bmfont.fnt", "reset"), callBack);
            resetBtn->setTag((int)menuTag::RESET);
            
            auto copyBtn = MenuItemLabel::create(Label::createWithBMFont("fonts/bmfont.fnt", "copy"), callBack);
            auto moveBtm = MenuItemLabel::create(Label::createWithBMFont("fonts/bmfont.fnt", "move"), callBack);
            
            auto copyMoveToggle = MenuItemToggle::createWithCallback(callBack, moveBtm, copyBtn, NULL);
            copyMoveToggle->setTag((int)menuTag::COPY_OR_MOVE_TOGGLE);
            
            
            _pMenu = Menu::create(resetBtn, copyMoveToggle, NULL);
            _pMenu->alignItemsHorizontally();
            Node::addChild(_pMenu,10,10);
        }
    }
    
}
void Board::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    for (auto touch : touches)
    {
        for (auto obj : getChildren())
        {
            if (obj == nullptr || obj->getParent() == nullptr  || obj == _pMenu)
            {
                continue;
            }
            bool bContains =
            obj->getBoundingBox().containsPoint(touch->getLocation());
            obj->setColor(Color3B::WHITE);
            if (bContains)
            {
                selectItem((Item*)obj, touch->getLocation());
            }
        }
    }
}

void Board::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    for (auto touch : touches)
    {
        if (_pShadowItem)
        {
            _pShadowItem->setPosition(touch->getLocation());
        }
        for (auto obj : getChildren())
        {
            if (obj != _pShadowItem && _pShadowItem )
            {
                bool bContgaines = obj->getBoundingBox().containsPoint(_pShadowItem->getPosition());
                if (bContgaines)
                {
                    obj->setColor(Color3B::YELLOW);
                }
                else
                {
                    if (obj!=_pSelectItem)
                    {
                        obj->setColor(Color3B::WHITE);
                    }
                }
                
            }
        }
    }
    
}
void Board::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    if (_pShadowItem && _pSelectItem)
    {
        for (auto obj : getChildren())
        {
            if (obj != _pShadowItem )
            {
                bool bContgaines = obj->getBoundingBox().containsPoint(_pShadowItem->getPosition());
                if (bContgaines)
                {
                    switch (_eMode)
                    {
                        case mode::MOVE:
                        {
                            bool bItem1 = _itemList.find((Item*)obj) != _itemList.end();
                            bool bItem2 = _itemList.find(_pSelectItem) != _itemList.end();
                            if (bItem1 & bItem2)
                            {
                                auto pos1 = obj->getPosition();
                                auto pos2 = _pSelectItem->getPosition();
                                if (itemMoveTo((Item*)obj, pos2))
                                {
                                    itemMoveTo(_pSelectItem, pos1);
                                    getChildren().swap(obj, _pSelectItem);
                                    
                                    _itemList.swap((Item*)obj, (Item*)_pSelectItem);
                                }
                            }

                        }
                            break;
                        case mode::COPY:
                        {
                            if (obj!=_pMenu)
                            {
                                auto item = (Item*)obj;
                                item->setStringValueWithColorChange(_pSelectItem->getStringValue());
                            }
                        }
                        default:
                            break;
                    }

                }
                obj->setColor(Color3B::WHITE);
                
            }
        }
        unSelectItem();
    }

}


void Board::selectItem(Item *item)
{
    selectItem(item, item->getPosition());
}
void Board::selectItem(Item *item, cocos2d::Vec2 selectPos)
{
    if (item->getActionByTag(AC_MOVE))
    {
        return;
    }
    item->setColor(Color3B::RED);
    _pSelectItem = item;
    if (_pSelectItem->getStringValue().length()>0)
    {
        _pShadowItem = Item::create(_pSelectItem    ->getStringValue());
        _pShadowItem->setPosition(selectPos);
        _pShadowItem->setOpacity(220);
        _pShadowItem->retain();
        _pShadowItem->setLocalZOrder(10);
        auto action = RepeatForever::create
        (
         Sequence::create
         (EaseBounceIn::create(RotateTo::create(1.0f, Vec3(0, 0, 10))),
          DelayTime::create(0.5f),
          EaseBounceIn::create(RotateTo::create(1.0f, Vec3(0, 0, -10))),
          DelayTime::create(0.5f),
          NULL)
        );
        action->setTag(AC_ROTATE3D);
        _pSelectItem->stopAllActions();
        _pSelectItem->runAction(action);
        addChild(_pShadowItem);
        
    }
    
}
void Board::unSelectItem()
{
    if (_pSelectItem)
    {
        _pSelectItem->setColor(Color3B::WHITE);
        _pSelectItem->setRotation3D(Vec3::ZERO);
        _pSelectItem->stopActionByTag(AC_ROTATE3D);
        _pSelectItem = nullptr;
    }
    if (_pShadowItem)
    {
        removeChild(_pShadowItem);
    }
    CC_SAFE_RELEASE_NULL(_pSelectItem);
    writeMappingFile();
}
bool Board::itemMoveTo(Item *item, cocos2d::Vec2 pos)
{
    
    if (item->getActionByTag(AC_MOVE) || (Node*)item == _pMenu)
    {
        return false;
    }
    
    auto action =
    Spawn::create
    (
     Sequence::create
     (
      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexUp, this)),
      EaseExponentialInOut::create(MoveTo::create(2.0f, pos)),
      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexDown, this)),
      NULL
      ),
     Sequence::create
     (EaseExponentialIn::create(ScaleTo::create(0.5f, 1.5f)),
      DelayTime::create(1.0f),
      EaseExponentialOut::create(ScaleTo::create(0.5f, 1.0f)),
      NULL),
     NULL
     );
    action->setTag(AC_MOVE);
    item->runAction(action);
    return true;
}
void Board::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    
}

void Board::addChild(Item *child)
{
    int count = getChildrenCount();
    Vec2 pos = Vec2::ZERO;
    if (count<_posList.size())
    {
        pos = _posList.at(count);
    }
    if (child!=_pShadowItem)
    {
        _itemList.pushBack(child);
    }
    child->setPosition(pos);
    Node::addChild(child);
    _stringList.push_back(child->getStringValue());
}


void Board::itemZindexUp(cocos2d::Node *sender)
{
//    sender->setLocalZOrder(100);
}
void Board::itemZindexDown(cocos2d::Node *sender)
{
//    sender->setLocalZOrder(0);
}


void Board::resetItemPosition()
{
    selectItem(_itemList.at(0));
    unSelectItem();
    _itemList.clear();
    for (int i=0; i<getChildrenCount(); i++)
    {
        auto node = getChildren().at(i);
        if ( node == _pMenu || i >_posList.size())
        {
            continue;
        }
        Item* item = (Item*)getChildren().at(i);
        item->stopAllActions();
        itemMoveTo(item, _posList.at(i));
        _itemList.pushBack(item);
        item->setStringValueWithColorChange(_stringList.at(i));
    }
    
}

void Board::menuCallBack(cocos2d::Ref *sender)
{
    menuTag tag = (menuTag)((Node*)sender)->getTag();
    switch (tag) {
        case menuTag::RESET:
            resetItemPosition();
            runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create(CC_CALLBACK_0(Board::resetItemPosition, this)), NULL));
            break;
        case menuTag::COPY_OR_MOVE_TOGGLE:
        {
            auto toggleMenu = (MenuItemToggle*)sender;
            int index = toggleMenu->getSelectedIndex();
            _eMode = (mode)index;
            switch (_eMode)
            {
                case mode::COPY:
                    setColor(Color3B::YELLOW);
                    break;
                case mode::MOVE:
                    setColor(Color3B::WHITE);
                    break;
                default:
                    break;
            }
            
        }
            
        default:
            break;
    }
}


void Board::writeMappingFile()
{
    
    std::string result;
    std::string jsonResult ;
    int index = 0;
    int index2 = 0;
    
    //output string make
    
    for (int i=0; i<4; i++)
    {
        result.append(StringUtils::format("\n[%d] = {",index2));
        for (int j=0; j<4; j++)
        {
            result.append("\n {");
            for (int k=0; k<12; k++)
            {
                auto stringValue = _itemList.at(index)->getStringValue();
                
                result.append(stringValue);
                if (k<11)
                {
                    result.append(", ");
                }
                index++;
            }
            result.append("}");
        }
        result.append("\n},");
        index2++;
    }
    
    //json string make
    
    jsonResult.append("[");
    for (auto item :  _itemList)
    {
        auto stringValue ="\""+item->getStringValue()+"\",";
        jsonResult.append(stringValue);
    }
    jsonResult.append("\" \"]");
    
    
    //file output
    auto path = FileUtils::getInstance()->getWritablePath();
    {
        std::ofstream file;
        file.open(path+"/output.txt", std::ios::out);
        file<< result;
        file.close();
    }
    
    //json output
    {
        std::ofstream file;
        file.open(path+"/save.json", std::ios::out);
        file<< jsonResult;
        file.close();
    }

}