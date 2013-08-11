/*
 * PopUpListener.h
 *
 *  Created on: 8 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef POPUPLISTENER_H_
#define POPUPLISTENER_H_

class PopUp;

class PopUpListener
{
	public:
		virtual void itemSelected(PopUp *object, int valSelected)=0;
};


#endif /* POPUPLISTENER_H_ */
