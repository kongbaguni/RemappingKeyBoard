//
//  MainScene.h
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#ifndef __Remapping__MainScene__
#define __Remapping__MainScene__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class MainScene : public Layer
{
public:
    CREATE_FUNC(MainScene);
    static Scene* createScene();
private:
    MainScene();
    virtual ~MainScene();
    virtual bool init();
    
    
};

#endif /* defined(__Remapping__MainScene__) */
