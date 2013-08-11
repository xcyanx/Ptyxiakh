/*
 * HypertextScreen.h
 *
 *  Created on: 2 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef HYPERTEXTSCREEN_H_
#define HYPERTEXTSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include "Screens/TextScreen.h"
#include "Screens/CameraScreen.h"
#include "../Common/DataStruct.h"
#include "Screens/CameraListener.h"
#include "Screens/TextScreen.h"
#include "Screens/CameraScreen.h"
#include "../GPS/GPSListener.h"

using namespace NativeUI;


class HypertextScreen: public StackScreen, public StackScreenListener, public ButtonListener,
					   public Cam::CameraListener
{
	public:
		HypertextScreen(Data &data);
		~HypertextScreen();

	private:
		virtual void stackScreenScreenPopped(StackScreen *stackScreen, Screen *fromScreen, Screen *toScreen);
		virtual void buttonClicked(Widget *button);
		virtual void Notify(int type);

	private:
		VerticalLayout *_Layout;
		Screen *screen;
		Button *_Button[3];
		TextScreen *_TextScreen;
		CameraScreen *_CameraScreen;
		Data &data;


	private:
		enum {Cam, Photo, Text};
};


#endif /* HYPERTEXTSCREEN_H_ */
