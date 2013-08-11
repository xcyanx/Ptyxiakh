/*
 * PopUp.cpp
 *
 *  Created on: 8 Σεπ 2012
 *      Author: Σταμάτης
 */

#include "PopUp.h"
#include "PopUpListener.h"

#define max(a,b) a<b?b:a;

PopUp::PopUp(): MAUI::ListBox(0, 0, 0, 0, NULL)
{
	_ItemSkin = NULL;
	_Font = NULL;


	PopUp::ListBox::setBackgroundColor(MAPUtil::Color::black.val());
	PopUp::ListBox::setAutoSize(true);
	PopUp::ListBox::setOrientation(MAUI::ListBox::LBO_VERTICAL);


	maxWidth = -1;
	maxHeigth = 0;

	createdSkin = false;
	createdFont = false;

	_Caller = NULL;
}

PopUp::~PopUp()
{
	//delete _Layout;
	//The layout is deleted by his parent.
	MAUtil::Environment::getEnvironment().removePointerListener(this);

	_Listeners.clear();

	if(createdSkin)
	{
		delete _ItemSkin;
	}

	if(createdFont)
	{
		delete _Font;
	}
}


void PopUp::addPopUpListener(PopUpListener *listener)
{
	NativeUI::addListenerToVector(_Listeners, listener);
}

void PopUp::removePopUpListener(PopUpListener *listener)
{
	NativeUI::removeListenerFromVector(_Listeners, listener);
}

void PopUp::applyItemSkin(MAHandle selectedImg, MAHandle unselectedImg, int x1, int x2, int y1, int y2,
						  bool selectedTrans, bool unselectedTrans)
{
	if(_ItemSkin != NULL && createdSkin)
	{
		delete _ItemSkin;
		//_ItemSkin = NULL;
	}

	createdSkin = true;
	_ItemSkin = new MAUI::WidgetSkin(selectedImg, unselectedImg, x1, x2, y1, y2, selectedTrans, unselectedTrans);
}

void PopUp::applyItemSkin(MAUI::WidgetSkin *WidgetSkin)
{
	if(_ItemSkin != NULL && createdSkin)
	{
		delete _ItemSkin;
		//_ItemSkin = NULL;
	}

	createdSkin = false;
	_ItemSkin = WidgetSkin;
}

void PopUp::hide()
{
	//_ListBox->clear();

	if(MAUI::Engine::getSingleton().isOverlayShown())
	{
		MAUI::Engine::getSingleton().hideOverlay();
	}

	MAUI::Environment::getEnvironment().removePointerListener(this);
}

void PopUp::show(int x, int y)
{
	//_ListBox->setParent(_Parent);

	if(MAUI::Engine::getSingleton().isOverlayShown())
	{
		MAUI::Engine::getSingleton().hideOverlay();
	}

	PopUp::ListBox::setPosition(x, y);

	//_ListBox->setPosition(x, y);
	//_Parent->add(_ListBox);

	MAUI::Engine::getSingleton().showOverlay(0 , 0, this);

	MAUI::Environment::getEnvironment().addPointerListener(this);
}


void PopUp::show(MAUI::Widget *widget)
{
	setCaller(widget);

	int sizeX = widget->getWidth()/2 + widget->getPosition().x;
	int sizeY = widget->getHeight()/2 + widget->getPosition().y;

	PopUp::show(sizeX, sizeY);
}

void PopUp::setCaller(MAUI::Widget *widget)
{
	_Caller = widget;
}

MAUI::Widget* PopUp::getCaller()
{
	return _Caller;
}

MAUI::Widget* PopUp::getChildByTag(MAUtil::String tag)
{
	MAUtil::Dictionary< const MAUtil::String,
		MAUtil::Pair<const MAUtil::String, MAUI::Widget*> >::Iterator it = _Options.find(tag);

	if(it == _Options.end() && tag != _Options.end()->first)
		return NULL;

	return it->second;
}

void PopUp::setItemFont(MAUI::Font *Font)
{
	if(_Font != NULL && createdFont)
	{
		delete _Font;
		//_Font = NULL;
	}

	createdFont = false;
	_Font = Font;
}

void PopUp::setItemFont(MAHandle Font)
{
	if(_Font != NULL && createdFont)
	{
		delete _Font;
	}

	createdFont = true;
	_Font = new MAUI::Font(Font);
}

void PopUp::addOption(MAUtil::String option)
{
	MAUI::Label *temp = new MAUI::Label(0, 0, 0, 0, NULL);

	_Options[option] = temp;

	if(_ItemSkin != NULL)
	{
		temp->setSkin(_ItemSkin);
	}

	if(_Font != NULL)
	{
		temp->setFont(_Font);
	}

	temp->setBackgroundColor(MAPUtil::Color::black.val());
	temp->setCaption(option);
	temp->setAutoSizeX(true);
	temp->setAutoSizeY(true);
	temp->setSelected(false);


	PopUp::ListBox::add(temp);

	//temp->setPosition(0, maxHeigth);

	MAExtent size = temp->getFont()->getStringDimensions(option.c_str(), option.size());


	maxWidth = max(maxWidth, EXTENT_X(size));
	maxHeigth = maxHeigth + EXTENT_Y(size);


	PopUp::ListBox::setWidth(maxWidth);
	PopUp::ListBox::setHeight(maxHeigth);
}

void PopUp::pointerPressEvent(MAPoint2d p)
{
	MAUtil::Vector<MAUI::Widget*> childs = PopUp::ListBox::getChildren(); //_ListBox->getChildren();

	for(int i = 0; i < childs.size(); i++)
	{
		if(childs[i]->contains(p.x, p.y))
		{
			childs[i]->setSelected(true);

			for(int j = 0; j < _Listeners.size(); j++)
			{
				_Listeners[j]->itemSelected(this, i);
			}
		}
	}
}

void PopUp::pointerMoveEvent(MAPoint2d p)
{
	//empty
}

void PopUp::pointerReleaseEvent(MAPoint2d p)
{
	MAUtil::Vector<MAUI::Widget*> childs = PopUp::ListBox::getChildren(); //_ListBox->getChildren();

	for(int i = 0; i < childs.size(); i++)
	{
		childs[i]->setSelected(false);
	}
}
