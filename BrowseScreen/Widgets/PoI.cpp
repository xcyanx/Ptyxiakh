/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "PoI.h"
#include <MAHeaders.h>
//#include "../std.h"

PoI::PoI(int x, int y, int width, int height, MAUI::Widget* parent)
   : MAUI::MapPin(x, y, width, height, parent)
{
	mImage = new MAUI::Image(0, 0, 10, 20, this, true, true, MAPPIN);
	mImage->update();

	mCaption = new MAUI::Label(mImage->getWidth(), 0, this->getWidth() - mImage->getWidth(), mImage->getHeight(), this);
	mCaption->setFont(new MAUI::Font(MY_FONT));
	mCaption->setCaption("Hi");
	mCaption->setDrawBackground(false);
	mCaption->setAutoSizeX(true);
	mCaption->setAutoSizeY(true);
	mCaption->setHorizontalAlignment(MAUI::Label::HA_CENTER);
	mCaption->setVerticalAlignment(MAUI::Label::VA_CENTER);
	mCaption->setPaddingLeft(4);
	mCaption->setPaddingRight(4);
	mCaption->setPaddingTop(2);
	mCaption->setPaddingBottom(2);
	this->setDrawBackground(false);

	MAUtil::Environment::getEnvironment().addPointerListener(this);

	_popUp = NULL;
}

PoI::~PoI()
{
	MAUtil::Environment::getEnvironment().removePointerListener(this);
}

void PoI::disable(bool disable)
{
	if(disable)
	{
		MAUtil::Environment::getEnvironment().removePointerListener(this);
	}
	else
	{
		MAUtil::Environment::getEnvironment().addPointerListener(this);
	}
}

void PoI::drawWidget()
{
	mCaption->update();
}

int PoI::getXOffset()
{
	if(mImage != NULL)
		return mImage->getWidth() * -1;
	else
		return 0;
}

int PoI::getYOffset()
{
	return mImage->getHeight() * -1;
}

MAUI::Label* PoI::getCaption()
{
	return mCaption;
}

void PoI::pointerPressEvent(MAPoint2d point)
{
	if(PoI::contains(point.x, point.y) && _popUp != NULL)
		_popUp->show(this);
}

void PoI::pointerMoveEvent(MAPoint2d point)
{

}

void PoI::pointerReleaseEvent(MAPoint2d point)
{

}

void PoI::itemSelected(PopUp *object, int valSelected)
{

}

void PoI::setPopUp(PopUp *popUp)
{
	_popUp = popUp;

	//_popUp->addPopUpListener(this);
}
