/*
 * Button.cpp
 *
 *  Created on: 8 Οκτ 2012
 *      Author: Σταμάτης
 */


#include "Button.h"
#include "ButtonListener.h"


MAUI::Button::Button(MAUtil::String caption):Label(0, 0, 0, 0, this)
{
	Label::setCaption(caption);
	Label::setAutoSizeX(true);
	Label::setAutoSizeY(true);
	Label::setSelected(false);
	Label::setHorizontalAlignment(HA_CENTER);
	Label::setVerticalAlignment(VA_CENTER);

	MAUI::Environment::getEnvironment().addPointerListener(this);

	buttonClicked = false;
	//visible = true;

	enabled = true;
}

MAUI::Button::Button(int x, int y, int width, int height, Widget *parent)
	:Label(x, y, width, height, parent)
{
	MAUI::Environment::getEnvironment().addPointerListener(this);

	buttonClicked = false;
	//visible = true;
	enabled = true;
}

void MAUI::Button::setCaption(const MAUtil::String &caption)
{
	Label::setCaption(caption);
	Label::setSelected(false);
	Label::setHorizontalAlignment(HA_CENTER);
	Label::setVerticalAlignment(VA_CENTER);
}

void MAUI::Button::addButtonListener(MAUI::ButtonListener *listener)
{
	NativeUI::addListenerToVector(_Listeners, listener);
}

void MAUI::Button::removeButtonListener(MAUI::ButtonListener *listener)
{
	NativeUI::removeListenerFromVector(_Listeners, listener);
}

/*void MAUI::Button::setVisible(bool visible)
{
	Button::visible = visible;
	//Button::setDirty(true);

	if(visible)
	{
		MAUI::Environment::getEnvironment().addPointerListener(this);
	}
	else
	{
		MAUI::Environment::getEnvironment().removePointerListener(this);
	}

	//MAUI::Engine::getSingleton().repaint();

	Button::draw(true);
}*/


void MAUI::Button::setEnabled(bool enabled)
{
	Button::enabled = enabled;

	Label::setEnabled(true);
}


MAUI::Button::~Button()
{
	MAUI::Environment::getEnvironment().removePointerListener(this);

	_Listeners.clear();
}

void MAUI::Button::pointerPressEvent(MAPoint2d p)
{
	if(Button::contains(p.x, p.y))
	{
		buttonClicked = true;

		Button::setSelected(true & enabled);
	}
	else
	{
		buttonClicked = false;
		Button::setSelected(false);
	}
}

void MAUI::Button::pointerMoveEvent(MAPoint2d p)
{
	if(Button::contains(p.x, p.y))
	{
		Button::setSelected(true & enabled);
	}
	else
	{
		Button::setSelected(false);
	}
}

void MAUI::Button::pointerReleaseEvent(MAPoint2d p)
{
	if(!enabled)
	{
		Button::setSelected(false);
		return;
	}

	if(buttonClicked && Button::contains(p.x, p.y))
	{
		for(int i = 0; i < _Listeners.size(); i++)
		{
			Button::setSelected(false);
			_Listeners[i]->buttonPressed(this);
		}
	}

	buttonClicked = false;
}

