/*
 * LogIn.cpp
 *
 *  Created on: 27 Αυγ 2012
 *      Author: Σταμάτης
 */


#include "LogIn.h"
#include "../MainScreen.h"
#include <mavsprintf.h>

#define LOG lprintfln

LogIn::LogIn(MainScreen *obj):Screen(), mConnection(this)
{
	_Layout = new NativeUI::VerticalLayout();

	_Layout->fillSpaceHorizontally();
	_Layout->fillSpaceVertically();

	LogIn::Screen::setMainWidget(_Layout);


	_Username = new NativeUI::EditBox();

	_Username->fillSpaceHorizontally();
	_Username->wrapContentVertically();
	_Username->setPlaceholder("Enter the username.");
	_Username->setMaxLength(19);
	//_EditBox->setText("Enter a filename.");
	_Layout->addChild(_Username);
	_Username->addEditBoxListener(this);


	_Password = new NativeUI::EditBox();

	_Password->fillSpaceHorizontally();
	_Password->wrapContentVertically();
	_Password->setPlaceholder("Enter the password.");
	_Password->setMaxLength(19);
	//_EditBox->setText("Enter a filename.");
	_Layout->addChild(_Password);
	_Password->addEditBoxListener(this);


	_Button = new NativeUI::Button();

	_Button->fillSpaceHorizontally();
	_Button->wrapContentVertically();
	_Button->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	_Button->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
	_Button->setText("Log In");
	_Layout->addChild(_Button);
	_Button->addButtonListener(this);

	this->obj = obj;
}

LogIn::~LogIn()
{
	_Username->removeEditBoxListener(this);
	_Password->removeEditBoxListener(this);
	_Button->removeButtonListener(this);

	mConnection.close();
}

void LogIn::connectFinished (Connection *conn, int result)
{
	if(result < 0)
	{
		maAlert("Magna Carta", "Connection: Connection to the server failed. Try again.", "Ok", NULL, NULL);

		mConnection.close();

		_Button->setEnabled(true);

		return;
	}

	packet = new LogInPacket();

	packet->PacketID = BasicPacket::ACC_VERIFICATION;
	strcpy(((LogInPacket*)packet)->username, _Username->getText().c_str());
	strcpy(((LogInPacket*)packet)->password, _Password->getText().c_str());

	LOG("username->%s\n", ((LogInPacket*)packet)->username);
	LOG("password->%s\n", ((LogInPacket*)packet)->password);
	LOG("packet ID->%d\n", packet->PacketID);

	conn->write(packet, sizeof(LogInPacket));
}

void LogIn::connReadFinished(Connection *conn, int result)
{
	LOG("Read packet id->%d\n", packet->PacketID);

	if(result < 0)
	{
		maAlert("Magna Carta", "Connection: Read failed. Try again.", "Ok", NULL, NULL);

		mConnection.close();

		_Button->setEnabled(true);

		delete packet;

		return;
	}

	//LogIn::result = packet->PacketID;

	//LOG("Read packet dumb->%s\n", packet->dumb);

	mConnection.close();

	_Button->setEnabled(true);

	if(packet->PacketID != BasicPacket::ACC_VERIFIED)
	{
		maAlert("Magna Carta", "Invalid username or password.", "Ok", NULL, NULL);

		delete packet;

		_Button->setEnabled(true);
	}
	else
	{
		LOG("Guid1: %ul", packet->guid.Data1);
		LOG("Guid2: %hu", packet->guid.Data2);
		LOG("Guid3: %hu", packet->guid.Data3);
		LOG("Guid4: %u", packet->guid.Data4);

		delete packet;

		obj->pop();
	}


	//obj->pop();
}

void LogIn::connWriteFinished(Connection *conn, int result)
{
	if(result < 0)
	{
		maAlert("Magna Carta", "Connection: Write failed. Try again.", "Ok", NULL, NULL);

		mConnection.close();

		_Button->setEnabled(true);

		delete packet;

		return;
	}

	delete packet;

	LOG("Login: Packet write finished successfully\n");
	LOG("Login: Size of packet is %d\n", sizeof(BasicPacket));

	packet = new BasicPacket();

	conn->read(packet, sizeof(BasicPacket));
}

void LogIn::buttonClicked(NativeUI::Widget *button)
{
	//obj->pop();
	//Authenticate the user.

	button->setEnabled(false);

	/*if(mConnection.isOpen())
	{
		return;
	}*/

	mConnection.connect("socket://46.177.45.100:3014");
}

void LogIn::editBoxReturn(NativeUI::EditBox *editBox)
{
	editBox->hideKeyboard();
}
