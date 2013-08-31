/*
 * MainScreen.cpp
 *
 *  Created on: 28 Ιουλ 2012
 *      Author: Σταμάτης
 */

#include "MainScreen.h"
#include <mavsprintf.h>

#define URL "socket://79.167.58.87:3015"


void MainScreen::buttonClicked(NativeUI::Widget *button)
{
	if(_Button[Options] == button)
	{
		//mapLoaded = true;
		//MainScreen::StackScreen::push(optionScreen);
		//MainScreen::StackScreen::setVisible(false);
		//_GPS->addGPSListener(optionScreen);
		_EditBox->addEditBoxListener(optionScreen);
		optionScreen->show();
	}
	else if(_Button[Hyper] == button)
	{
		//hyperScreen = new HypertextScreen();
		MainScreen::StackScreen::push(hyperScreen);
	}
	else if(_Button[Start] == button)
	{
		static bool clicked = false;

		if(clicked == false) //GPS starts tracking.
		{
			if(_EditBox->getText().size() == 0)
			{
				maAlert("Magna Carta", "Add a file name into the textbox.", "Ok", NULL, NULL);
				return;
			}

			if(xml != NULL)
			{
				delete xml;

				xml = NULL;
			}

			xml = new XML(_EditBox->getText());

			optionScreen->getPinLayer()->clearLayer();
			optionScreen->getLineLayer()->clearLayer();
			optionScreen->getLonLatArray()->clear();
			deleteTop(0);

			loc_data.imagePath = "";
			loc_data.videoPath = "";
			loc_data.text = "";

			_GPS->gpsActivate();
			_GPS->addGPSListener(optionScreen);

			if(!_GPS->isActive())
			{
				MAUtil::String str = "GPS Error Code: ";
				maAlert("Magna Carta", (str+
						MAUtil::integerToString(_GPS->getErrorCode(), 10)).c_str(), "Ok", NULL, NULL);

				return;
			}


			_EditBox->setEnabled(false);
			_Button[Hyper]->setEnabled(true);
			_Button[Upload]->setEnabled(false);
			_Button[Start]->setText("Stop tracking");
			clicked = true;
			//Set code here to start the tracking using a timer module.
		}
		else //And here is stop tracking.
		{
			_Button[Hyper]->setEnabled(false);
			_EditBox->setEnabled(true);
			_Button[Start]->setText("Start tracking");

			_Button[Upload]->setEnabled(true);

			clicked = false;

			xml->finalize();

			//optionScreen->getLonLatArray()->clear();

			_GPS->removeGPSListener(optionScreen);
			_GPS->gpsStop();
			//Set code here to stop the tracking.
		}
	}
	else if(_Button[Upload] == button)
	{
		mConnection->close();

		mConnection->connect(URL);
	}
}

void MainScreen::connectFinished(Connection *conn, int result)
{
	if(result < 0)
	{
		maAlert("Magna Carta", "Error while trying to upload the data.", "Ok", NULL, NULL);

		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		mConnection->close();
	}

	_ActivityIndicator->setMessage("Uploading the xml data to the server.");
	_ActivityIndicator->show();

	packet = new NextPacketSize();

	((NextPacketSize*)packet)->PacketID = BasicPacket::DATA_UPLOAD;
	((NextPacketSize*)packet)->size = xml->getSize();

	maFileSeek(xml->getHandle(), 0, MA_SEEK_SET);

	mConnection->write(packet, sizeof(NextPacketSize));
}

void MainScreen::connWriteFinished(MAUtil::Connection *conn, int result)
{
	int size, remaining;

	remaining = maFileSize(xml->getHandle()) - maFileTell(xml->getHandle());

	if(result < 0)
	{
		maAlert("Magna Carta", "Error while trying to upload the data.", "Ok", NULL, NULL);

		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		mConnection->close();

		return;
	}

	if(remaining == 0)
	{
		_ActivityIndicator->hide();

		return;
	}

	size = 256 < remaining ? 256 : remaining;

	//delete buffer;

	if(buffer != NULL)
	{
		delete buffer;

		buffer = NULL;
	}

	buffer = new char[size];

	memset(buffer, NULL, size);

	if(!maFileRead(xml->getHandle(), buffer, size))
	{
		//MAUtil::String tmp = Base64::encode(buffer, xml->getSize());

		//delete buffer;

		//buffer = new char[tmp.size()+1];

		//memset(buffer, NULL, tmp.size()+1);

		//strcpy(buffer, tmp.c_str());

		//lprintfln("Writting %s", buffer);

		mConnection->write(buffer, size);
	}
	else
	{
		if(buffer != NULL)
		{
			delete buffer;

			buffer = NULL;
		}
	}
}

void MainScreen::editBoxEditingDidEnd(EditBox *editBox)
{
	editBox->hideKeyboard();
}

void MainScreen::deleteTop(int num)
{
	if(_ListView->countChildWidgets() > num)
	{
		for(int i = 0; i < _ListView->countChildWidgets(); i++)
		{
			NativeUI::Widget *temp = _ListView->getChild(i);


			_ListView->removeChild(temp);
			temp->markWidgetHandleAsDeleted();

			delete temp;
		}
	}
}

void MainScreen::editBoxEditingDidBegin(EditBox *editBox)
{
	//editBox->setText("");
	editBox->showKeyboard();
}


void MainScreen::editBoxReturn(EditBox *editBox)
{
	if(editBox->getText().size() == 0)
	{
		maAlert("Magna Carta", "Add a file name into the textbox.", "Ok", NULL, NULL);
		editBox->showKeyboard();
		return;
	}
	else
	{
		editBox->hideKeyboard();
	}


	if(editBox->getText().size() > FILENAME_MAX || editBox->getLastError().errorCode == MAW_RES_INVALID_STRING_BUFFER_SIZE)
	{
		maAlert("Magna Carta", "Use a shorter name.", "Ok", NULL, NULL);
		return;
	}


	//xml->CreateRoot();
}

Screen* MainScreen::getScreen()
{
	return this;
}

Option* MainScreen::getOptScr()
{
	return optionScreen;
}

MainScreen::MainScreen():StackScreen()
{
	//Set the StackScreenListener
	MAExtent tSize = maGetScrSize();

	int sizeX = EXTENT_X(tSize);
	int sizeY = EXTENT_Y(tSize);

	MainScreen::StackScreen::addStackScreenListener(this);

	_Layout = new NativeUI::VerticalLayout();

	_Layout->fillSpaceHorizontally();
	_Layout->fillSpaceVertically();

	//Set the root element that will be displayed as our main screen.
	screen = new Screen();
	screen->setMainWidget(_Layout);

	//MainScreen::StackScreen::push(screen);

	_ActivityIndicator = new WaitMessage();



	//Create an editbox for the filename.
	_EditBox = new NativeUI::EditBox();

	_EditBox->fillSpaceHorizontally();
	_EditBox->wrapContentVertically();
	_EditBox->setPlaceholder("Enter a filename.");
	//_EditBox->setText("Enter a filename.");
	_Layout->addChild(_EditBox);
	_EditBox->addEditBoxListener(this);

	//Create the 3 buttons of our application.
	_Button[Options] = new NativeUI::Button();

	_Button[Options]->fillSpaceHorizontally();
	_Button[Options]->wrapContentVertically();
	_Button[Options]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Options]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Options]->setText("Show a route");
	_Layout->addChild(_Button[Options]);
	_Button[Options]->addButtonListener(this);


	_Button[Hyper] = new NativeUI::Button();

	_Button[Hyper]->fillSpaceHorizontally();
	_Button[Hyper]->wrapContentVertically();
	_Button[Hyper]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Hyper]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Hyper]->setText("Add Hypertext to point");
	_Button[Hyper]->setEnabled(false);
	_Layout->addChild(_Button[Hyper]);
	_Button[Hyper]->addButtonListener(this);

	_HLayout = new NativeUI::HorizontalLayout();

	_HLayout->fillSpaceHorizontally();
	_HLayout->wrapContentVertically();
	_Layout->addChild(_HLayout);

	_Button[Start] = new NativeUI::Button();

	lprintfln("Screen width / 2: %d", sizeX/2);

	_Button[Start]->setWidth(sizeX/2);
	_Button[Start]->wrapContentVertically();
	_Button[Start]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Start]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Start]->setText("Start tracking");
	_HLayout->addChild(_Button[Start]);
	_Button[Start]->addButtonListener(this);

	_Button[Upload] = new NativeUI::Button();

	_Button[Upload]->setWidth(sizeX/2);
	_Button[Upload]->wrapContentVertically();
	_Button[Upload]->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Upload]->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button[Upload]->setText("Upload route.");
	_HLayout->addChild(_Button[Upload]);
	_Button[Upload]->addButtonListener(this);
	_Button[Upload]->setEnabled(false);



	//Create a label.
	NativeUI::Label *lv_label = new NativeUI::Label();

	lv_label->setText("GPS locations tracked:");
	lv_label->fillSpaceHorizontally();
	lv_label->wrapContentVertically();
	lv_label->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	lv_label->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Layout->addChild(lv_label);

	//Create the ListView object.
	_ListView = new NativeUI::ListView();
	_ListView->fillSpaceHorizontally();
	_ListView->fillSpaceVertically();
	_Layout->addChild(_ListView);

	//_ListViewItems = new MAUtil::Vector<ListViewItem*>();

	//Create the screens.
	optionScreen = new Option(this);

	previewScreen = new PreviewScreen(this);

	hyperScreen = new HypertextScreen(loc_data);

	logInScreen = new LogIn(this);
	MainScreen::StackScreen::push(logInScreen);

	_GPS = new GPS();
	_GPS->setAccError(20);
	_GPS->addGPSListener(this);
	//_GPS->addGPSListener(optionScreen);

	metersSeconds = true;
	//mapLoaded = false;
	//loc_data.init = false;
	//hyperScreen = NULL;

	mConnection = new MAUtil::Connection(this);

	packet = NULL;
	xml = NULL;
	buffer = NULL;
}

PreviewScreen* MainScreen::getPreviewScreen()
{
	return previewScreen;
}

void MainScreen::callPreviewScreen(int Screen)
{
	optionScreen->hide();
	_EditBox->removeEditBoxListener(optionScreen);
	//_GPS->removeGPSListener(optionScreen);

	previewScreen->setMain(Screen);
	MainScreen::StackScreen::push(previewScreen);
}

void MainScreen::stackScreenScreenPopped(StackScreen *stackScreen, Screen *fromScreen, Screen *toScreen)
{
	if(fromScreen == logInScreen)
	{
		MainScreen::StackScreen::push(this->screen);
	}

	if(fromScreen == previewScreen)
	{
		//_GPS->addGPSListener(optionScreen);
		_EditBox->addEditBoxListener(optionScreen);
		optionScreen->show();
	}
}

void MainScreen::pop()
{
	if(optionScreen->isVisible())
	{
		optionScreen->hide();
		_EditBox->removeEditBoxListener(optionScreen);
		//_GPS->removeGPSListener(optionScreen);
		return;
	}

	if(hyperScreen != NULL && hyperScreen->getStackSize() > 0)
	{
		if(hyperScreen->getStackSize() == 1)
		{
			MainScreen::StackScreen::pop();
			//delete hyperScreen;
		    //hyperScreen = NULL;

		    return;
		}


		hyperScreen->pop();
	}
	else
	{
		MainScreen::StackScreen::pop();
	}
}

MainScreen::~MainScreen()
{
	//Remove the event listeners.
	_Button[Options]->removeButtonListener(this);
	_Button[Hyper]->removeButtonListener(this);
	_Button[Start]->removeButtonListener(this);
	_EditBox->removeEditBoxListener(this);
	MainScreen::StackScreen::removeStackScreenListener(this);
	_GPS->removeGPSListener(this);
	_EditBox->removeEditBoxListener(optionScreen);
	_GPS->removeGPSListener(optionScreen);


	if(xml != NULL)
		delete xml;

	if(mConnection != NULL)
	{
		mConnection->close();

		delete mConnection;
	}

	if(packet != NULL)
		delete packet;

	//Delete the screens.
	delete hyperScreen;
	delete optionScreen;
	delete screen;
	delete logInScreen;
	delete previewScreen;
	delete _ActivityIndicator;
}

void MainScreen::updateList(MALocation loc)
{
	ListViewItem *item = new ListViewItem();
	MAUtil::String str;

	str = "Lat: ";
	str += MAUtil::doubleToString(loc.lat);
	str += "--";
	str += "Lon: ";
	str += MAUtil::doubleToString(loc.lon);

	item->setText(str);

	deleteTop(10);

	_ListView->addChild(item);
}


void MainScreen::firstPoint(GPS *gpsWidget, MALocation loc)
{
	loc_data.loc = loc;
	loc_data.time = MAPUtil::DateTime::now();

	//_ActivityIndicator->setMessage("Saving data to xml file.");
	//_ActivityIndicator->show();

	xml->WriteNode(loc_data);

	//_ActivityIndicator->hide();

	updateList(loc);

	//gpsWidget->changeCoords(loc);
}

void MainScreen::gpsEnd(GPS* gpsWidget, MALocation loc, MALocation old_loc)
{
	if(gpsWidget->coordsToMeters(loc, old_loc) >= 5)
	{
		gpsWidget->changeCoords(loc);
	}
}

void MainScreen::hasMoved(GPS* gpsWidget, MALocation loc, MALocation old_loc)
{
	if(gpsWidget->coordsToMeters(loc, old_loc) >= 5)
	{

		//lprintfln("coordsToMeters(MainScreen): %f", gpsWidget->coordsToMeters(loc, old_loc));
		//lprintfln("MainScreen(loc_data.img): %s", loc_data.imagePath.c_str());
		//lprintfln("MainScreen(loc_data.img): %s", loc_data.videoPath.c_str());

		//_ActivityIndicator->setMessage("Saving data to xml file.");
		//_ActivityIndicator->show();

		xml->WriteNode(loc_data);

		//_ActivityIndicator->hide();

		if(loc_data.imagePath != "" || loc_data.videoPath != "" || loc_data.text != "")
		{
			PoI *tmpPin = new PoI(0, 0, 12, 20, NULL);

			tmpPin->setLonLat(loc_data.loc.lon, loc_data.loc.lat);

			optionScreen->getPinLayer()->addPin(tmpPin);
		}


		loc_data.loc = loc;
		loc_data.time = MAPUtil::DateTime::now();
		loc_data.imagePath = "";
		loc_data.text = "";
		loc_data.videoPath = "";


		//gpsWidget->changeCoords(loc); //Update the coords in the gps module.
		updateList(loc); //and add them to the list widget.
	}
}


void MainScreen::compassOrientationChanged(GPS* gpsWidget, int azimuth)
{
	return;
}
