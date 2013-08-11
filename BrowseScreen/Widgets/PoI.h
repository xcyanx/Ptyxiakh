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

#ifndef POI_H_
#define POI_H_

#include <MAUI/Image.h>
#include <MAUI/Label.h>
//using namespace MAUI;

#include "MapPin.h"
#include "../../PopUp/PopUp.h"
#include "../../PopUp/PopUpListener.h"

class PoI : public MAUI::MapPin, private MAUtil::PointerListener//, public PopUpListener
{
	public:
		PoI(int x, int y, int width, int height, MAUI::Widget* parent = NULL);
		~PoI();

		int getXOffset();
		int getYOffset();
		void drawWidget();
		void setPopUp(PopUp *popUp);
		void disable(bool _disable = true);

		MAUI::Label* getCaption();

	private:
		virtual void itemSelected(PopUp *object, int valSelected);
		virtual void pointerPressEvent (MAPoint2d p);
		virtual void pointerMoveEvent (MAPoint2d p);
		virtual void pointerReleaseEvent (MAPoint2d p);

	private:
		MAUI::Image* mImage;
		MAUI::Label* mCaption;
		PopUp *_popUp;
};

#endif /* POI_H_ */
