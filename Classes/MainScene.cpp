//
//  MainScene.cpp
//  Remapping
//
//  Created by kongbaguni on 2015. 7. 24..
//
//

#include "MainScene.h"
#include "Board.h"
#include "Item.h"

MainScene::MainScene()
{
    
}
MainScene::~MainScene()
{
    
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    
    if (scene && layer)
    {
        scene->addChild(layer);
    }
    else
    {
        CC_SAFE_RELEASE_NULL(scene);
    }    
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto board = Board::create();
    addChild(board);
    
    std::string buttonList[] =
        {
            "KC_ESC",  "KC_Q",    "KC_W",    "KC_E",    "KC_R",    "KC_T",    "KC_Y",    "KC_U",    "KC_I",    "KC_O",    "KC_P",   "KC_BSPC",
            "KC_TAB",  "KC_A",    "KC_S",    "KC_D",    "KC_F",    "KC_G",    "KC_H",    "KC_J",    "KC_K",    "KC_L",    "KC_SCLN",  "KC_QUOT",
            "KC_LSFT", "KC_Z",    "KC_X",    "KC_C",    "KC_V",    "KC_B",    "KC_N",    "KC_M",    "KC_COMM", "KC_DOT",  "KC_SLSH", "KC_LSFT",
            "BL_STEP", "KC_LCTL", "KC_LALT", "KC_LGUI", "FUNC(2)",    "KC_SPC",   "KC_SPC",    "FUNC(1)",   "KC_LEFT", "KC_DOWN", "KC_UP",  "KC_RGHT"
        };
        
    for (int i=0; i<48; i++)
    {
        board->addChild(Item::create(buttonList[i]));
    }
    return true;
}