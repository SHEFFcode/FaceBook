#include "HelloWorldScene.h"
#include "FacebookAgent.h"

USING_NS_CC;

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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //Initialize the facebook SDK
    auto facebook = cocos2d::plugin::FacebookAgent::getInstance();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setScale(4);
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto closeItem2 = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::capture, this));
    closeItem2->setScale(4);
    
    closeItem2->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 - 300 ,
                                origin.y + closeItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, closeItem2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::afterCaptured(bool succeed, const std::string& outputFile)
{
    if (succeed)
    {
        // show screenshot
        auto sp = Sprite::create(outputFile);
        addChild(sp, 0, "screenshot");
        Size s = Director::getInstance()->getWinSize();
        sp->setPosition(s.width / 2, s.height / 2);
        sp->setScale(0.25);
    }
    else
    {
        log("Capture screen failed.");
    }
}

void HelloWorld::capture(Ref* pSender)
{
    utils::captureScreen(CC_CALLBACK_2(HelloWorld::afterCaptured, this),"CaptureScreenTest.png");
    SHEFFpath = FileUtils::getInstance()->getWritablePath();
    SHEFFpath += "CaptureScreenTest.png";
    CCLOG("save image to %s", SHEFFpath.c_str());
}

std::string HelloWorld::sceenshot(std::string& filename)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    auto tex = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
    tex->setPosition((origin.x + visibleSize.width) / 2, (origin.y + visibleSize.height) / 2);
    tex->begin();
    Director::getInstance()->getRunningScene()->visit();
    tex->end();
    
    std::string imgPath = FileUtils::getInstance()->getWritablePath();
    if (imgPath.length() == 0) {
        return "";
    }
    
    bool ret = tex->saveToFile(filename, Image::Format::PNG);
    if (ret) {
        imgPath += filename;
        CCLOG("save image to %s", imgPath.c_str());
        return imgPath;
    }
    return "";
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::string fileName = "CaptureScreenTest.png";
    
    std::string imgPath = sceenshot(fileName);
    
    auto delay = DelayTime::create(1.0);
    auto share = CallFunc::create([=](){
        cocos2d::plugin::FacebookAgent::FBInfo params;
        params.insert(std::make_pair("dialog", "sharePhoto"));
        params.insert(std::make_pair("photo", imgPath));
        
        if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
        {
            cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret, std::string& msg){
                CCLOG("%s", msg.c_str());
            });
        }
        else
        {
            cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret, std::string& msg){
                CCLOG("%s", msg.c_str());
            });

        }
    });
    
    auto seq = Sequence::create(delay, share, nullptr);
    runAction(seq);

    
   #pragma Link Share
//    cocos2d::plugin::FacebookAgent::FBInfo params;
//    params.insert(std::make_pair("dialog", "share_link"));
//    params.insert(std::make_pair("name", "Facebook sharing implemented"));
//    params.insert(std::make_pair("caption", "If this appears, facebook support is implemented."));
//    params.insert(std::make_pair("description", "If this appears, facebook support is implemented."));
//    params.insert(std::make_pair("link", "http://www.yuriyshefer.com"));
//    
//    if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret ,std::string& msg){
//            CCLOG("%s", msg.c_str());
//        });
//    }
//    else
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->share(params, [=](int ret ,std::string& msg)
//                                            {
//                                                CCLOG("%s", msg.c_str());
//                                            });
//    }
#endif
}
