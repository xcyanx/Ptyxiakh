/*
 * PopUp.h
 *
 *  Created on: 8 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef POPUP_H_
#define POPUP_H_

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
//#include "PopUpListener.h"


class PopUpListener;

class PopUp:  private MAUI::PointerListener, public MAUI::ListBox
{
	public:
		PopUp();
		~PopUp();
		void addOption(MAUtil::String option);
		void show(int x, int y);
		void show(MAUI::Widget *widget);
		void hide();
		void addPopUpListener(PopUpListener *listener);
		void removePopUpListener(PopUpListener *listener);
		MAUI::Widget* getChildByTag(MAUtil::String tag);
		void setItemFont(MAHandle Font);
		void setItemFont(MAUI::Font *Font);
		void applyItemSkin(MAUI::WidgetSkin *WidgetSkin);
		void applyItemSkin(MAHandle selectedImg, MAHandle unselectedImg, int x1,
						   int x2, int y1, int y2, bool selectedTrans = true, bool unselectedTrans = true);
		void setCaller(MAUI::Widget *widget = NULL);
		MAUI::Widget* getCaller();
		//void setImgSelected(MAHandle Img);
		//void setImgUnselected(MAHandle Img);

	private:
		//virtual void itemSelected(MAUI::ListBox *sender, MAUI::Widget *selectedWidget, MAUI::Widget *unselectedWidget);
		//virtual void blocked(MAUI::ListBox*, int val);
		//virtual void popedUp(PopUp *object, int valSelected);
		virtual void pointerPressEvent(MAPoint2d p);
		virtual void pointerMoveEvent(MAPoint2d p);
		virtual void pointerReleaseEvent(MAPoint2d p);


	private:
		MAUtil::Map<MAUtil::String, MAUI::Widget*> _Options;
		MAUtil::Vector<PopUpListener*> _Listeners;
		//MAUI::Widget *_Parent;
		MAUI::WidgetSkin *_ItemSkin;
		MAUI::Font		*_Font;
		MAUI::Widget *_Caller;
		//MAHandle _Img[2];
		//MAUI::WidgetSkin *_PopUpSkin;
		//MAUI::ListBox *_ListBox;
		int maxWidth;
		int maxHeigth;
		bool createdFont;
		bool createdSkin;
};

#endif /* POPUP_H_ */
