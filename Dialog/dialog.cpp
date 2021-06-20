#include "dialog.h"
#include "cocos2d.h"

USING_NS_CC;

Dialog::Dialog() :
	m_title(NULL),
	m_content(NULL),
	m_content_fontSize(0),
	m_title_fontSize(0),
	m_callback(NULL),
	m_size(0, 0)
{
}

Dialog::~Dialog()
{
	CC_SAFE_RELEASE(background);
	CC_SAFE_RELEASE(menu);
}

Scene* Dialog::createScene()
{
	auto scene = Scene::create();
	auto layer = Dialog::create();
	scene->addChild(layer);
	return scene;
}

bool Dialog::init()
{
	if (!Layer::init()) {
		return false;
	}
	menu = Menu::create();
	node = Vector<Node*>();
	visiableSize = Director::getInstance()->getVisibleSize();
	return true;
}

bool Dialog::addButton(const char* normalImage, const char* selectedImage, const char* button_title, int tag, Vec2 position)
{
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(Dialog::ButtonCallback, this));
	menuImage->setTag(tag);
	menuImage->setPosition(position);

    Size imenu = menuImage->getContentSize();
    
    Label* Label = Label::createWithTTF(button_title, "", 20);
	Label->setColor(Color3B(0, 0, 0));
	Label->setPosition(Point(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(Label);
	Menu* menu = Menu::create(menuImage, NULL);
	node.pushBack(menu);
	return true;
}

bool Dialog::addButton(const char* normalImage, const char* selectedImage, int tag, Vec2 position)
{
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(Dialog::ButtonCallback, this));
	menuImage->setTag(tag);
	Menu* menu = Menu::create(menuImage, NULL);
	menu->setPosition(position);
	node.pushBack(menu);
	return true;
}

void Dialog::setCallbackFunc(CallFuncN*  callfun)
{
	m_callback = callfun;
}

void Dialog::setBackground(const char* backgroundImage)
{

	this->backgroundImage = backgroundImage;
}

void Dialog::ButtonCallback(Ref *pSender)
{
    if (m_callback) {
        m_callback->execute();
    }
	this->removeFromParentAndCleanup(true);
}

void Dialog::onEnter()
{
	Layer::onEnter();
	background = Sprite::create(backgroundImage);
	background->setPosition(visiableSize.width / 2, visiableSize.height / 2);
	background->setScale(1.0f);
	addChild(background, 1);
	m_size = background->getContentSize();

	for (const auto& node2 : node)
	{
		addChild(node2, 2);
	}
	
	FiniteTimeAction* action = Sequence::create(
		ScaleTo::create(0.2, 0.2),
		ScaleTo::create(0.4, 0.4),
		ScaleTo::create(0.6, 0.6),
		ScaleTo::create(0.8, 0.8), NULL);
	this->runAction(action);
}

void Dialog::onExit()
{
	Layer::onExit();
}

