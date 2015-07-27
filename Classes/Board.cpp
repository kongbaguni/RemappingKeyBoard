//
//  Board.cpp
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#include "Board.h"
Board::Board():
_pSelectNode(nullptr)
{
    _posList.clear();
    for (int i=4 ; i>0; i--)
    {
        float y = i*16;
        for (int j=1; j<13; j++)
        {
            float x = j*32;
            _posList.push_back(Vec2(x, y));
        }
    }
}
Board::~Board()
{
    
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

void Board::sortItems()
{
}

void Board::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    for (auto touch : touches)
    {
        for (auto obj : getChildren())
        {
            if (obj == nullptr || obj->getParent() == nullptr )
            {
                continue;
            }
            bool bContains =
            obj->getBoundingBox().containsPoint(touch->getLocation());
            obj->setColor(Color3B::WHITE);
            if (bContains)
            {
                obj->setColor(Color3B::RED);
                _pSelectNode = (Item*)obj;
                if (_pSelectNode->getStringValue().length()>0)
                {
                    _pShadowItem = Item::create(_pSelectNode    ->getStringValue());
                    _pShadowItem->setPosition(touch->getLocation());
                    _pShadowItem->setOpacity(100);
                    _pShadowItem->retain();
                    _pShadowItem->setLocalZOrder(10);
                    addChild(_pShadowItem);
                    
                }
                
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
            if (obj != _pShadowItem )
            {
                bool bContgaines = obj->getBoundingBox().containsPoint(_pShadowItem->getPosition());
                if (bContgaines)
                {
                    obj->setColor(Color3B::YELLOW);
                }
                else
                {
                    if (obj!=_pSelectNode)
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
    if (_pShadowItem)
    {
        for (auto obj : getChildren())
        {
            if (obj != _pShadowItem )
            {
                bool bContgaines = obj->getBoundingBox().containsPoint(_pShadowItem->getPosition());
                if (bContgaines)
                {
                    auto pos1 = obj->getPosition();
                    auto pos2 = _pSelectNode->getPosition();
                    _pSelectNode->runAction
                    (
                     Sequence::create
                     (
                      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexUp, this)),
                      EaseExponentialInOut::create(MoveTo::create(2.0f, pos1)),
                      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexDown, this)),
                      NULL
                      )
                     );
                    _pSelectNode->runAction
                    (Sequence::create
                     (EaseExponentialIn::create(ScaleTo::create(0.5f, 1.5f)),
                      DelayTime::create(1.0f),
                      EaseExponentialOut::create(ScaleTo::create(0.5f, 1.0f)),
                      NULL));
                    
                    obj->runAction
                    (
                     Sequence::create
                     (
                      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexUp, this)),
                      EaseExponentialInOut::create(MoveTo::create(2.0f, pos2)),
                      CallFuncN::create(CC_CALLBACK_1(Board::itemZindexDown, this)),
                      NULL
                     )
                     );
                    obj->runAction
                    (Sequence::create
                     (EaseExponentialIn::create(ScaleTo::create(0.5f, 1.5f)),
                      DelayTime::create(1.0f),
                      EaseExponentialOut::create(ScaleTo::create(0.5f, 1.0f)),
                      NULL));
                    
                    _itemList.swap((Item*)obj, (Item*)_pSelectNode);
                }
                obj->setColor(Color3B::WHITE);
                
            }
        }
        removeChild(_pShadowItem);
        CC_SAFE_RELEASE_NULL(_pShadowItem);
    }

}
void Board::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    
}

void Board::addChild(cocos2d::Node *child)
{
    int count = getChildrenCount();
    Vec2 pos = Vec2::ZERO;
    if (count<_posList.size())
    {
        pos = _posList.at(count);
    }
    if (child!=_pShadowItem)
    {
        _itemList.pushBack((Item*)child);
    }
    child->setPosition(pos);
    Node::addChild(child);
}


void Board::itemZindexUp(cocos2d::Node *sender)
{
    sender->setLocalZOrder(100);
}
void Board::itemZindexDown(cocos2d::Node *sender)
{
    sender->setLocalZOrder(0);
}
