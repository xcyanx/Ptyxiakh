/*
 * Text.cpp
 *
 *  Created on: 3 Αυγ 2012
 *      Author: Σταμάτης
 */

#include "TextScreen.h"

TextScreen::TextScreen(NativeUI::StackScreen &scr,Data &datam):Screen(), data(datam), _scr(scr)
{
	//Init the Layout.
	_Layout = new NativeUI::VerticalLayout();
	_Layout->fillSpaceHorizontally();
	_Layout->fillSpaceVertically();
	TextScreen::Screen::setMainWidget(_Layout);

	//Init the editbox.
	MAExtent scrSize = maGetScrSize();
	int Height = EXTENT_Y(scrSize);

	_EditBox = new NativeUI::EditBox();

	_EditBox->fillSpaceHorizontally();
	_EditBox->setHeight((int)(0.9*Height));
	_EditBox->setPlaceholder("Add your text here.");
	_EditBox->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
	_Layout->addChild(_EditBox);

	//Init the button.
	_Button = new NativeUI::Button();

	_Button->fillSpaceHorizontally();
	_Button->fillSpaceVertically();
	_Button->setText("Add text");
	_Button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Layout->addChild(_Button);
	_Button->addButtonListener(this);
}

TextScreen::~TextScreen()
{
	_Button->removeButtonListener(this);
	//_EditBox->hideKeyboard();
}

void TextScreen::show()
{
	TextScreen::Screen::show();

	//_EditBox->showKeyboard();
}

void TextScreen::buttonClicked(Widget *button)
{
	data.text = _EditBox->getText();

	_EditBox->hideKeyboard();

	_EditBox->setText("");

	_scr.pop();

	//maAlert("Magna Carta", "Text assosiated with the point successfully", "Ok", NULL, NULL);
}
