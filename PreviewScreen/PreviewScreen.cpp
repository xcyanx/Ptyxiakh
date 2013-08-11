/*
 * PreviewScreen.cpp
 *
 *  Created on: 13 Οκτ 2012
 *      Author: Σταμάτης
 */


#include "PreviewScreen.h"

PreviewScreen::PreviewScreen(MainScreen *mScreen): Screen()
{
	scrHeight = EXTENT_Y(maGetScrSize());

	_Layout = new NativeUI::VerticalLayout();
	_VideoView = new NativeUI::VideoView();
	_Image = new NativeUI::Image();
	_EditBox = new NativeUI::EditBox();
	_Button = new NativeUI::Button();
	_LastWidget = NULL;

	_Layout->fillSpaceHorizontally();
	_Layout->fillSpaceVertically();

	_VideoView->fillSpaceHorizontally();
	_VideoView->setHeight((int)(0.9*scrHeight));

	_Image->fillSpaceHorizontally();
	_Image->setHeight((int)(0.9*scrHeight));

	_Button->addButtonListener(this);
	_Button->setText("Go back");
	_Button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button->fillSpaceHorizontally();
	_Button->setHeight((int)(0.1*scrHeight));

	_EditBox->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
	_EditBox->fillSpaceHorizontally();
	_EditBox->setHeight((int)(0.9*scrHeight));

	PreviewScreen::mScreen = mScreen;

	PreviewScreen::setMainWidget(_Layout);
}

PreviewScreen::~PreviewScreen()
{
	if(_LastWidget != _Image)
	{
		delete _Image;
	}

	if(_LastWidget != _VideoView)
	{
		delete _VideoView;
	}

	if(_LastWidget != _EditBox)
	{
		delete _EditBox;
	}

	_Button->removeButtonListener(this);
}

void PreviewScreen::buildVideoScreen()
{
	//maAlert("Called", NULL, "Ok", NULL, NULL);
	if(_LastWidget != NULL)
	{
		_Layout->removeChild(_LastWidget);
		_Layout->removeChild(_Button);
	}

	_Layout->addChild(_VideoView);
	_Layout->addChild(_Button);

	_LastWidget = _VideoView;
}

void PreviewScreen::buildImageScreen()
{
	//maAlert("Called", NULL, "Ok", NULL, NULL);
	if(_LastWidget != NULL)
	{
		_Layout->removeChild(_LastWidget);
		_Layout->removeChild(_Button);
	}

	_Layout->addChild(_Image);
	_Layout->addChild(_Button);

	_LastWidget = _Image;
}

void PreviewScreen::buildTextScreen()
{
	//maAlert("Called", NULL, "Ok", NULL, NULL);
	if(_LastWidget != NULL)
	{
		_Layout->removeChild(_LastWidget);
		_Layout->removeChild(_Button);
	}

	_Layout->addChild(_EditBox);
	_Layout->addChild(_Button);

	_LastWidget = _EditBox;
}

void PreviewScreen::setMain(int ScreenNum)
{
	switch(ScreenNum)
	{
		case VideoScr:
			buildVideoScreen();
			break;
		case ImageScr:
			buildImageScreen();
			break;
		case TextScr:
			buildTextScreen();
			break;
	}
}

void PreviewScreen::setImageData(MAHandle img)
{
	_Image->setImage(img);
}

void PreviewScreen::setVideoData(MAUtil::String url)
{
	_VideoView->setURL(url);
	_VideoView->play();
}

void PreviewScreen::setTxtData(MAUtil::String txt)
{
	_EditBox->setText(txt);
}

void PreviewScreen::buttonClicked(Widget *button)
{
	mScreen->pop();
}
