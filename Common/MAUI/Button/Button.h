/*
 * Button.h
 *
 *  Created on: 8 Οκτ 2012
 *      Author: Σταμάτης
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <MAUI/Screen.h>
#include <MAUI/Layout.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Widget.h>
#include <MAUI/Font.h>
#include <MAP/Color.h>
#include <MAHeaders.h>
#include <NativeUI/Widgets.h>
#include <MAUI/InputManager.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Geometry.h>

namespace MAUI
{
	class ButtonListener;

	class Button: public Label, private MAUI::PointerListener
	{
		public:
			Button(MAUtil::String Caption);
			Button(int x, int y, int width, int height, Widget *parent);
			~Button();
			void setCaption(const MAUtil::String &caption);
			void addButtonListener(MAUI::ButtonListener *listener);
			void removeButtonListener(MAUI::ButtonListener *listener);
			void setEnabled(bool enabled = true);
			//void setVisible(bool visible = true);

		private:
			virtual void pointerPressEvent(MAPoint2d p);
			virtual void pointerMoveEvent(MAPoint2d p);
			virtual void pointerReleaseEvent(MAPoint2d p);
			//void drawWidget();

		private:
			MAUtil::Vector<ButtonListener*> _Listeners;
			bool buttonClicked;
			//bool visible;
			bool enabled;
	};
}


#endif /* BUTTON_H_ */
