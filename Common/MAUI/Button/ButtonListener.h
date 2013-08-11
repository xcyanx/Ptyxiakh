/*
 * ButtonListener.h
 *
 *  Created on: 8 Οκτ 2012
 *      Author: Σταμάτης
 */

#ifndef BUTTONLISTENER_H_
#define BUTTONLISTENER_H_

namespace MAUI
{
	class Button;

	class ButtonListener
	{
		public:
			virtual void buttonPressed(MAUI::Button *object)=0;
	};
}


#endif /* BUTTONLISTENER_H_ */
