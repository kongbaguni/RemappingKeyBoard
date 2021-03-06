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
#include <json/document.h>

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
            "BL_STEP", "KC_LCTL", "KC_LALT", "KC_LGUI", "FUNC(2)",    "KC_SPC",   "KC_SPC",    "FUNC(1)",   "KC_LEFT", "KC_DOWN", "KC_UP",  "KC_RGHT",
            
            "KC_ESC", "KC_Q",    "KC_W",    "KC_F",    "KC_P",      "KC_G",    "KC_J",    "KC_L",    "KC_U",    "KC_Y",    "KC_SCLN",  "KC_BSPC",
            "KC_TAB", "KC_A",    "KC_R",    "KC_S",    "KC_T",      "KC_D",    "KC_H",    "KC_N",    "KC_E",    "KC_I",    "KC_O",     "KC_QUOT",
            "KC_LSFT", "KC_Z",    "KC_X",    "KC_C",   "KC_V",      "KC_B",    "KC_K",    "KC_M",    "KC_COMM", "KC_DOT",  "KC_SLSH", "KC_LSFT",
            "KC_FN3", "KC_LCTL", "KC_LALT", "KC_LGUI", "FUNC(2)",    "KC_SPC",   "KC_SPC",    "FUNC(1)",   "KC_LEFT", "KC_DOWN", "KC_UP",  "KC_RGHT",
            
            "KC_GRV",  "KC_1",    "KC_2",    "KC_3",    "KC_4",       "KC_5",       "KC_6",       "KC_7",       "KC_8",    "KC_9",    "KC_0",    "KC_BSPC",
            "KC_TRNS", "FUNC(3)", "FUNC(4)", "LSFT(RSFT(KC_PAUSE))", "KC_TRNS",    "KC_COMM",    "KC_DOT",     "KC_MINS",    "KC_EQL",  "KC_LBRC", "KC_RBRC", "KC_BSLS",
            "KC_F11",  "KC_F12",  "KC_F13",  "KC_F14",               "KC_F15",     "KC_TAB",     "KC_BSPC",     "KC_F16",    "KC_F17",  "KC_F18",  "KC_F19",  "KC_F20",
            "KC_TRNS", "KC_TRNS", "KC_TRNS", "KC_TRNS",              "KC_TRNS",    "KC_ENT",     "KC_ENT",    "FUNC(1)",     "KC_MNXT", "KC_VOLD", "KC_VOLU", "KC_MPLY",

            "S(KC_GRV)",   "S(KC_1)",    "S(KC_2)",    "S(KC_3)",                "S(KC_4)",    "S(KC_5)",     "S(KC_6)",     "S(KC_7)",      "S(KC_8)",    "S(KC_9)",    "S(KC_0)",    "KC_BSPC",
            "KC_TRNS",     "FUNC(3)",    "FUNC(4)",    "LSFT(RSFT(KC_PAUSE))",   "KC_TRNS",    "KC_COMM",     "KC_DOT",      "S(KC_MINS)",   "S(KC_EQL)",  "S(KC_LBRC)", "S(KC_RBRC)", "S(KC_BSLS)",
            "KC_F1",       "KC_F2",      "KC_F3",      "KC_F4",                  "KC_F5",      "KC_TAB",      "KC_BSPC",     "KC_F6",        "KC_F7",      "KC_F8",      "KC_F9",      "KC_F10",
            "KC_TRNS",     "KC_TRNS",    "KC_TRNS",    "KC_TRNS",                "FUNC(2)",    "KC_BSPC",     "KC_BSPC",     "KC_TRNS",      "KC_MNXT",    "KC_VOLD",    "KC_VOLU",    "KC_MPLY",

        };
    
    //load save.json
    {
        auto filePath = FileUtils::getInstance()->getWritablePath()+"/save.json";
        bool bFileExist = FileUtils::getInstance()->isFileExist(filePath);
        if(bFileExist)
        {
            auto data = FileUtils::getInstance()->getStringFromFile(filePath);
            rapidjson::Document document;
            document.Parse(data.c_str());
            if(!document.HasParseError() && document.IsArray())
            {
                for(int i=0; i<document.Size(); i++)
                {
                    std::string str = document[i].GetString();
                    if(str.compare(" ")!=0)
                    {
                        board->addChild(Item::create(str));
                    }
                }
                
            }
        }
        else
        {
            for (int i=0; i<sizeof(buttonList)/sizeof(std::string); i++)
            {
                board->addChild(Item::create(buttonList[i]));
            }
            for (int i=0; i<32; i++)
            {
                board->addChild(Item::create("KC_TRNS"));
            }
            
        }
        
    }
        
    return true;
}