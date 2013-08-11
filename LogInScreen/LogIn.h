/*
 * LogIn.h
 *
 *  Created on: 27 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef LOGIN_H_
#define LOGIN_H_


#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <MAUtil/Connection.h>
#include "../Common/Packets/Packet.h"


using namespace MAUtil;

class MainScreen;


class LogIn: public NativeUI::Screen, private NativeUI::ButtonListener,
		     private NativeUI::EditBoxListener, private ConnectionListener
{
	public:
		LogIn(MainScreen *obj);
		~LogIn();

	private:
		virtual void buttonClicked(NativeUI::Widget *button);
		virtual void editBoxReturn(NativeUI::EditBox *editBox);
		virtual void connectFinished(Connection *conn, int result);
		virtual void connWriteFinished(Connection *conn, int result);
		virtual void connReadFinished(Connection *conn, int result);

	private:
		NativeUI::EditBox *_Username;
		NativeUI::EditBox *_Password;
		NativeUI::Button  *_Button;
		Connection 		  mConnection;
		MainScreen 	      *obj;
		NativeUI::VerticalLayout *_Layout;
		BasicPacket       *packet;
		int 			  result;
		//char 			  linebuffer[1024];
};

#endif /* LOGIN_H_ */
