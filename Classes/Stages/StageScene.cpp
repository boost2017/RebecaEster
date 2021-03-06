#include "StageScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* StageScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    auto stageLayer = StageScene::create();
    stageLayer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(stageLayer);
    stageLayer->toggleDebug();
    
    return scene;
}

bool StageScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    createBackgroundWithAnimation();
    createPlayer();
    
    // add label points
    auto label = Label::createWithTTF("Points: 0", "fonts/Marker Felt.ttf", 20);
    label->setPosition(Vec2(visibleSize.width - label->getContentSize().width - 10, origin.y + visibleSize.height - label->getContentSize().height));
    label->setAlignment(TextHAlignment::RIGHT);
    label->setAnchorPoint(Vec2(0.0f, 1.0f));
    addChild(label);
    
    // add edge
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);
    
    // joystick data
    int joystickOffset = 10;
    
    Rect joystickBaseDimensions = Rect(0, 0, 64.0f, 64.0f);
    Point joystickBasePosition  = Point(origin.x + (joystickBaseDimensions.size.width / 2) + joystickOffset, origin.y + (joystickBaseDimensions.size.height / 2) + joystickOffset);
    
    Rect accelButtonDimensions = Rect(0, 0, 64.0f, 64.0f);
    Point accelButtonPosition  = Point(origin.x + visibleSize.width - (joystickBaseDimensions.size.width / 2) - joystickOffset,origin.y + (joystickBaseDimensions.size.height / 2) + joystickOffset);
    
    // add joystick base
    SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::create();
    SneakyJoystick *joystick = new SneakyJoystick();
    joystick->initWithRect(joystickBaseDimensions);
    joystickBase->setBackgroundSprite(cocos2d::Sprite::create("res/joystick/dpadDown.png"));
    joystickBase->setThumbSprite(cocos2d::Sprite::create("res/joystick/joystickDown.png"));
    joystickBase->setJoystick(joystick);
    joystickBase->setPosition(joystickBasePosition);
    leftJoystick = joystickBase->getJoystick();
    addChild(joystickBase);
    
    // add joystick button
    SneakyButtonSkinnedBase *action1ButtonBase = SneakyButtonSkinnedBase::create();
    SneakyButton *button1 = new SneakyButton();
    button1->initWithRect(accelButtonDimensions);
    action1ButtonBase->setDefaultSprite(cocos2d::Sprite::create("res/joystick/action1Down.png"));
    action1ButtonBase->setActivatedSprite(cocos2d::Sprite::create("res/joystick/action1Down.png"));
    action1ButtonBase->setPressSprite(cocos2d::Sprite::create("res/joystick/action1Up.png"));
    action1ButtonBase->setButton(button1);
    action1ButtonBase->setPosition(accelButtonPosition);
    action1Button = action1ButtonBase->getButton();
    addChild(action1ButtonBase);
    
    // camera follow the player
    runAction(Follow::create(player->getSprite(), Rect(0, 0, visibleSize.width, visibleSize.height)));
    
    // joystick listener
    leftJoystick->onVelocityChanged = [=](SneakyJoystick* eventJoystick, Point oldValue, Point newValue){
        cocos2d::log("Velocity x: %f y: %f", newValue.x, newValue.y);
    };
    
    action1Button->onFire = [=](SneakyButton *button) {
        cocos2d::log("Fire in the hole!!!");
    };
    
    /*
    auto bottomBody = PhysicsBody::createBox(Size(visibleSize.width, 20));
    bottomBody->setDynamic(false);
    auto bottomNode = Node::create();
    bottomNode->setPosition(Point(visibleSize.width/2, 50));
    bottomNode->setPhysicsBody(bottomBody);
    this->addChild(bottomNode);
     */
    
    /*
    auto physicsBodyTest = PhysicsBody::createBox(Size(40, 40));
    auto physicsBodyTestNode = Sprite::create("CloseNormal.png");
    physicsBodyTestNode->addComponent(physicsBodyTest);
    addChild(physicsBodyTestNode);
    physicsBodyTestNode->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    */
    
    scheduleUpdate();
    
    return true;
}

void StageScene::createPlayer()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    player = Character::create("wizard");
    //player->showRunAnimation(true);
    player->getSprite()->setPosition(Vec2(winSize.width/2, winSize.height/2));
    //player->getSprite()->setScale(0.5f);
    
    addChild(player->getSprite());
}

void StageScene::createBackgroundWithAnimation()
{
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto winSize = Director::getInstance()->getWinSize();
    
    auto backGround = Sprite::create("res/stages/stage1/background.png", Rect(0.0f, 0.0f, winSize.width, winSize.height));
    auto backGround2 = Sprite::create("res/stages/stage1/background.png", Rect(0.0f, 0.0f, winSize.width, winSize.height));
    
    parallaxNode = InfiniteParallaxNode::create();
    backGround->setAnchorPoint(Point::ZERO);
    backGround2->setAnchorPoint(Point::ZERO);
    
    parallaxNode->addChild(backGround, 0, Vec2(1.0f, 0.0f), Vec2(0.0f, 0.0f));
    parallaxNode->addChild(backGround2, 0, Vec2(1.0f, 0.0f), Vec2(winSize.width, 0.0f));
    
    addChild(parallaxNode);
}

void StageScene::update(float delta)
{
    Layer::update(delta);
    
    parallaxNode->setPosition(parallaxNode->getPosition() + Vec2(-2.0f, 0.0f));
    parallaxNode->updatePosition(Vec2(-2.0f, 0.0f));
    
    // update the player
    if (player)
    {
        player->updateVelocity(leftJoystick->getVelocity());
        
        if (action1Button->getIsActive())
        {
            player->actionButtonPressed(1);
        }
    }
}

void StageScene::toggleDebug()
{
    debugDraw = !debugDraw;
    getScene()->getPhysicsWorld()->setDebugDrawMask(debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
}

void StageScene::setPhysicsWorld(PhysicsWorld *physicsWorld)
{
    this->physicsWorld = physicsWorld;
}
