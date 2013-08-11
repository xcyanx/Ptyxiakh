/*
 * Text.h
 *
 *  Created on: 3 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <NativeUI/StackScreen.h>
#include "../../Common/DataStruct.h"
//#include "../../MainScreen.h"

using namespace NativeUI;

class TextScreen: public Screen, public ButtonListener
{
	public:
		TextScreen(NativeUI::StackScreen &scr,Data &data);
		void show();
		~TextScreen();

	private:
		virtual void buttonClicked(Widget *button);

	private:
		VerticalLayout *_Layout;
		EditBox *_EditBox;
		Button *_Button;
		NativeUI::StackScreen &_scr;
		Data &data;
};


#endif /* TEXT_H_ */
