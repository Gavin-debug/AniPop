#pragma once
#ifndef _Dialog_H_
#define _Dialog_H_

#include "cocos2d.h"

USING_NS_CC;

class Dialog :public Layer
{
public:

	Size m_size;
	
	Dialog();
	
	~Dialog();
	
	static Scene* createScene();
	
	virtual bool init();
	
    bool addButton(const char* normalImage, const char* selectedImage,const char* button_title, int tag, Vec2 position);
	
	bool addButton(const char* normalImage, const char* selectedImage, int tag, Vec2 position);
	
    void setCallbackFunc(CallFuncN* callfun);
	
    void setBackground(const char* backgroundImage);
	
	void  ButtonCallback(Ref *pSender);

	virtual void onEnter();
	
	virtual void onExit();
	
    CallFuncN*  m_callback;

	CREATE_FUNC(Dialog);

private:
	
	Menu* menu;
	
	Vector<Node*> node;
	
	const char* m_title;
	
	const char* m_content;
	
	int m_title_fontSize;
	
	int m_content_fontSize;
	
	Sprite* sprite_m;
	
	Sprite* background;
	
	Size visiableSize;
	
	const char* backgroundImage;
};

#endif //_DIALOG_H_
