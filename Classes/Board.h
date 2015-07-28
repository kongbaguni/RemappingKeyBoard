//
//  Board.h
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#ifndef __Remapping__Board__
#define __Remapping__Board__

#include <stdio.h>
#include "cocos2d.h"
#include "Item.h"
USING_NS_CC;

class Board : public LayerColor
{
    
public:
    CREATE_FUNC(Board);
    virtual void addChild(Item * child);
protected:
    
    typedef enum
    {
        AC_MOVE  = 123,
    }actionTag;
    
    enum class menuTag
    {
        RESET,
        COPY_OR_MOVE_TOGGLE
    };
    
    enum class mode
    {
        MOVE,
        COPY
    };
    mode _eMode;

    Board();
    virtual ~Board();
    virtual bool init();

    
    virtual void onEnter();
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);
    
    void sortItems();
    void showMenu();

private:
    void menuCallBack(Ref* sender);
    Item* _pSelectItem;
    Item* _pShadowItem;
    Menu* _pMenu;
    /** 아이템 위치값 리스트*/
    std::vector<Vec2> _posList;
    /** 최초 설정된 키 스트링 값 목록 */
    std::vector<std::string> _stringList;
    
    Vector<Item*> _itemList;
    void itemZindexUp(Node* sender);
    void itemZindexDown(Node* sender);
    
    void selectItem(Item* item);
    void selectItem(Item* item, Vec2 selectPos);
    
    void unSelectItem();
    
    bool itemMoveTo(Item* item, Vec2 pos);
    
    void resetItemPosition();
    
    void writeMappingFile();
    
};

#endif /* defined(__Remapping__Board__) */
