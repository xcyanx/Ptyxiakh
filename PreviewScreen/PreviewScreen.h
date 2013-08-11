/*
 * PreviewScreen.h
 *
 *  Created on: 13 Οκτ 2012
 *      Author: Σταμάτης
 */

#include <maapi.h>
#include <MAUtil/util.h>
#include <MAFS/File.h>
#include <MAP/DateTime.h>
#include <MAUI/Screen.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "../MainScreen.h"

#ifndef PREVIEWSCREEN_H_
#define PREVIEWSCREEN_H_

class PreviewScreen: public NativeUI::Screen, private NativeUI::ButtonListener
{
	public:
		PreviewScreen(MainScreen *mScreen);
		~PreviewScreen();
		void setMain(int ScreenNum);
		void setImageData(MAHandle img);
		void setVideoData(MAUtil::String url);
		void setTxtData(MAUtil::String txt);

	private:
		void buildVideoScreen();
		void buildImageScreen();
		void buildTextScreen();
		void buttonClicked(Widget *button);

	public:
		enum {VideoScr, ImageScr, TextScr};

	private:
		NativeUI::VerticalLayout *_Layout;
		NativeUI::VideoView *_VideoView;
		NativeUI::Image *_Image;
		NativeUI::EditBox *_EditBox;
		NativeUI::Button *_Button;
		NativeUI::Widget *_LastWidget;
		MainScreen *mScreen;
		int scrHeight;
};


#endif /* PREVIEWSCREEN_H_ */
