#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onExit();
    
    void menuStartCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__
