/**
	\mainpage WaitMessage
	\brief ###Shows message with title and AcitivtyIndicator###

	\image html WaitMessage.png	"[WaitMessage] in action"
	\image latex WaitMessage.png	"[WaitMessage] in action"
	
	This project was created to simplify common task - showing message with wait indicator.    
	
	There is possibility to change title, text of message, to hide and show message.	
**/

/**
 * \file WaitMessage.h
 * \brief Contains WaitMessage class.
 */

#ifndef WAITMESSAGE_H_
#define WAITMESSAGE_H_

#include <NativeUI/Widgets.h>
#include <MAUtil/String.h>

using namespace MAUtil;
using namespace NativeUI;

/**
 * \brief Class to show wait message.
 * \version 1.0
 * \author fatINbraIN
 *
 * Allow to show, hide wait message, change title and text of message.
 */
class WaitMessage {
public:
	/**
	 * \brief Constructor.
	 * @param title Title of WaitMessage.
	 * @param message Text of WaitMessage.
	 */
	WaitMessage(String title ="Please, wait...", String message = "text message");

	/**
	 * \brief Destructor.
	 */
	~WaitMessage();

	/**
	 * \brief Show WaitMessage with previously set title and message.
	 */
	void show();

	/**
	 * \brief Hide WaitMessage.
	 */
	void hide();

	/**
	 * \brief Set text of message.
	 * @param message New text of message.
	 */
	void setMessage(String message);

	/**
	 * \brief Set new title of message.
	 * @param title New title.
	 */
	void setTitle(String title);

private:
	String message_;	/**<\brief Text of message*/
	String title_;		/**<\brief Title of message*/
	Dialog* dia_;		/**<\brief Dialog widget to contain WaitMessage*/
	Label* lbMessage_;	/**<\brief Label widget to contain text of message*/
	ActivityIndicator* ai_;	/**<\brief Widget indicating wait process*/

	/**
	 * \brief Create layout of WaitMessage.
	 */
	void createLayout();
};

#endif /* WAITMESSAGE_H_ */
