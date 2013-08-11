/*
 * HypertextScreen.cpp
 *
 *  Created on: 2 Αυγ 2012
 *      Author: Σταμάτης
 */


#include "HypertextScreen.h"
#include <mavsprintf.h>

HypertextScreen::HypertextScreen(Data &datam):StackScreen(), data(datam)
{
	HypertextScreen::StackScreen::addStackScreenListener(this);


	//Init the layout widget.
	_Layout = new NativeUI::VerticalLayout();
	_Layout->fillSpaceHorizontally();
	_Layout->fillSpaceVertically();

	//Init the screen widget.
	screen = new NativeUI::Screen();
    screen->setMainWidget(_Layout);

    //Push the main screen into the stack screen.
    HypertextScreen::StackScreen::push(screen);

    //Add the child widgets in the _Layout.
    //In this case the buttons.
    _Button[Cam] = new NativeUI::Button();

    _Button[Cam]->fillSpaceHorizontally();
    _Button[Cam]->wrapContentVertically();
    _Button[Cam]->setText("Camera");
    _Button[Cam]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
    _Button[Cam]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
    _Layout->addChild(_Button[Cam]);
    _Button[Cam]->addButtonListener(this);


    _Button[Photo] = new NativeUI::Button();

    _Button[Photo]->fillSpaceHorizontally();
    _Button[Photo]->wrapContentVertically();
    _Button[Photo]->setText("Photograph");
    _Button[Photo]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
    _Button[Photo]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
    _Layout->addChild(_Button[Photo]);
    _Button[Photo]->addButtonListener(this);


    _Button[Text] = new NativeUI::Button();

    _Button[Text]->fillSpaceHorizontally();
    _Button[Text]->wrapContentVertically();
    _Button[Text]->setText("Text");
    _Button[Text]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
    _Button[Text]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
    _Layout->addChild(_Button[Text]);
    _Button[Text]->addButtonListener(this);

    //Create the screens.
    _TextScreen = new TextScreen(*this, this->data);
    _CameraScreen = new CameraScreen();

    _CameraScreen->AddCamListener(this);
    //this->data = data;
}

HypertextScreen::~HypertextScreen()
{
	_Button[Cam]->removeButtonListener(this);
	_Button[Photo]->removeButtonListener(this);
	_Button[Text]->removeButtonListener(this);
	_CameraScreen->RemoveCamListener(this);
	HypertextScreen::StackScreen::removeStackScreenListener(this);

	//Delete the screens.
	delete _TextScreen;
	delete _CameraScreen;
	delete screen;
}

void HypertextScreen::stackScreenScreenPopped(StackScreen *stackScreen, Screen *fromScreen, Screen *toScreen)
{
	/*if(fromScreen == NULL)
	{
		return;
	}

	delete fromScreen;
	fromScreen = NULL;*/
}

void HypertextScreen::buttonClicked(Widget *button)
{
	if(_Button[Cam] == button)
	{
		_CameraScreen->LaunchCamera();
		//Add stuff here to get the path.
	}
	else if(_Button[Photo] == button)
	{
		_CameraScreen->LaunchPhoto();
		//Get the path..
	}
	else if(_Button[Text] == button)
	{
		//TextScreen *text = new TextScreen();
		HypertextScreen::StackScreen::push(_TextScreen);
	}
}



void HypertextScreen::Notify(int type)
{
	MAUtil::String pathname;

	if(type == CameraScreen::_CAMERA_)
	{
		_CameraScreen->GetPathname(pathname);
		data.videoPath = pathname;

		lprintfln("Notify(vid): %s", data.videoPath.c_str());
	}
	else if(type == CameraScreen::_PHOTO_)
	{
		_CameraScreen->GetPathname(pathname);
		data.imagePath = pathname;

		lprintfln("Notify(img): %s", data.imagePath.c_str());
	}
}
