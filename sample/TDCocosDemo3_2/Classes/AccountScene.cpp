/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "AccountScene.h"
#include "MissionScene.h"
#include "AppDelegate.h"


using namespace cocos2d;

extern std::string s_strAppKey;

enum {
    CREATE_USER_ID = 0,
    ADD_LEVEL,
    UPDATE_GAME_SERVER,
    UPDATE_USER_NAME,
    UPDATE_GENDER,
    UPDATE_TYPE
};

typedef struct tagEventMenuItem {
    std::string id;
    int tag;
} EventMenuItem;

static EventMenuItem s_EventMenuItem[] = {
    {"User ID : userid_%d",        CREATE_USER_ID},
    {"User Level : level_%d"   , ADD_LEVEL},
    {"Game Server : server_%d", UPDATE_GAME_SERVER},
    {"User Name : name_%d", UPDATE_USER_NAME},
    {"Gender : gender_%d", UPDATE_GENDER},
    {"Type : %d", UPDATE_TYPE}

};

static int _indexes[] = {0, 0, 0, 0, 0, 0};

AccountLayer::AccountLayer():mTDGAaccount(NULL) {
}

AccountLayer::~AccountLayer() {
}

Scene* AccountLayer::scene() {
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    AccountLayer *layer = AccountLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool AccountLayer::init() {
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }

    //this->setKeypadEnabled(true);

    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();

    auto title = LabelTTF::create("Account", "Arial", 24);
	title->setPosition( Point(visibleSize.width / 2, visibleSize.height - 20));
	this->addChild(title, 1);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *pCloseItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(AccountLayer::menuCloseCallback, this));
    pCloseItem->setPosition( Point(pCloseItem->getContentSize().width / 2 , 20) );

    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    pMenu->setPosition( Point::ZERO );
    this->addChild(pMenu, 1);
    
    // add next menu
    MenuItemImage *pNextItem = MenuItemImage::create(
                                                          "f1.png",
                                                          "f2.png",
                                                          CC_CALLBACK_1(AccountLayer::menuNextCallback,this));
    pNextItem->setPosition( Point(visibleSize.width - pNextItem->getContentSize().width / 2, 20) );
    
    // create menu, it's an autorelease object
    Menu* pNextMenu = Menu::create(pNextItem, NULL);
    pNextMenu->setPosition( Point::ZERO );
    this->addChild(pNextMenu, 1);

    float step = 35;
    float yPos = 0;
    for (int i = 0; i < sizeof(s_EventMenuItem)/sizeof(s_EventMenuItem[0]); i++) {
        char buf[64];
		sprintf(buf, s_EventMenuItem[i].id.c_str(), ++_indexes[i]);
        
        auto label = LabelTTF::create(buf, "Arial", 30);
        MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(AccountLayer::eventMenuCallback,this));
        pMenu->addChild(pMenuItem, 0, s_EventMenuItem[i].tag);
        yPos = visibleSize.height - step*i - 100;
        pMenuItem->setPosition( Point(visibleSize.width / 2, yPos));
    }

    return true;
}


void AccountLayer::eventMenuCallback(cocos2d::Ref* pSender) {
    auto pMenuItem = (MenuItemLabel*)pSender;
    char buf[64];
    int i = pMenuItem->getTag();
    
    log("DeviceId:%s", TDCCTalkingDataGA::getDeviceId());

    sprintf(buf, s_EventMenuItem[i].id.c_str(), ++_indexes[i]);
    LabelTTF* label = (LabelTTF*)pMenuItem->getLabel();
    label->setString(buf);
    switch (i) {
    case CREATE_USER_ID:
    	mTDGAaccount = TDCCAccount::setAccount(label->getString().c_str());
        break;
            
    case ADD_LEVEL:
        if (mTDGAaccount) mTDGAaccount->setLevel(_indexes[i]);
        break;
            
    case UPDATE_GAME_SERVER:
        if (mTDGAaccount) mTDGAaccount->setGameServer(label->getString().c_str());
        break;
            
    case UPDATE_USER_NAME:
        if (mTDGAaccount) mTDGAaccount->setAccountName(label->getString().c_str());
        break;

    case UPDATE_GENDER:
    	if (mTDGAaccount) mTDGAaccount->setGender(mTDGAaccount->kGenderMale);
    	break;

    case UPDATE_TYPE:
    	if (mTDGAaccount) mTDGAaccount->setAccountType(mTDGAaccount->kAccountType1);
    	break;

    default:
        break;
    }
}

void AccountLayer::menuCloseCallback(cocos2d::Ref* pSender) {
    TDCCTalkingDataGA::onKill();
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void AccountLayer::menuNextCallback(cocos2d::Ref* pSender) {
	Scene *pScene = MissionLayer::scene();
	CCDirector::getInstance()->pushScene(CCTransitionZoomFlipAngular::create(0.618f, pScene));
}

