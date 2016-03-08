#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TDCCTalkingDataGA.h"

#include "AccountScene.h"
#include "ItemScene.h"
#include "MissionScene.h"
#include "VirtualCurrencyScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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

HelloWorld::HelloWorld():mTDGAaccount(NULL) {
    
}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition( Point(closeItem->getContentSize().width / 2 , 50) );

    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    float step = 35;
    float yPos = 0;
    for (int i = 0; i < sizeof(s_EventMenuItem)/sizeof(s_EventMenuItem[0]); i++) {
        char buf[64];
        sprintf(buf, s_EventMenuItem[i].id.c_str(), ++_indexes[i]);
        
        auto label = LabelTTF::create(buf, "Arial", 24);
        MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(HelloWorld::menuEventCallback, this));
        menu->addChild(pMenuItem, 0, s_EventMenuItem[i].tag);
        yPos = visibleSize.height - step*i - 100;
        pMenuItem->setPosition( Point(visibleSize.width / 2, yPos));
    }
    
    // add next menu
    MenuItemImage *pNextItem = MenuItemImage::create("f1.png","f1.png",CC_CALLBACK_1(HelloWorld::menuNextCallback,this));
    pNextItem->setPosition( Point(visibleSize.width - pNextItem->getContentSize().width / 2, 50) );
    
    // create menu, it's an autorelease object
    Menu* pNextMenu = Menu::create(pNextItem, NULL);
    pNextMenu->setPosition( Point::ZERO );
    this->addChild(pNextMenu, 1);

    
    return true;

}

void HelloWorld::enterAccountScene(){
    
    log("enterAccountScene");
    Scene *scene = AccountLayer::creatScene();
    Director::getInstance()->pushScene(scene);
}

void HelloWorld::enterItemScene(){
    
}
void HelloWorld::enterMissionScene(){
    
}
void HelloWorld::enterVirtuslCurrencyScene(){
    
}

void HelloWorld::menuEventCallback(cocos2d::Ref* pSender){
    MenuItemLabel* pMenuItem = (MenuItemLabel*)pSender;
    char buf[64];
    int i = pMenuItem->getTag();
    
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

void HelloWorld::menuNextCallback(cocos2d::Ref* pSender) {
    HelloWorld::enterAccountScene();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
