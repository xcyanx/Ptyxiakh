/*
 * PinLayer.h
 *
 *  Created on: 2 Νοε 2012
 *      Author: Σταμάτης
 */

#ifndef PINLAYER_H_
#define PINLAYER_H_

#include "Widgets/MapLayer.h"
#include "MAHeaders.h"

#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>
#include <maapi.h>
#include <MAUI/Image.h>
#include <MAP/LonLat.h>

class PinLayer: public MapLayer
{
	public:
		typedef MAUtil::Vector<MAUI:: MapPin*> Pins;

	public:
		PinLayer();
		~PinLayer();

		void addPin(MAUI::MapPin* pin);
		void drawLayer(MAP::MapWidget* map);
		void activate(bool activate = true);
		void clearLayer();
		Pins getPins();

	private:
		Pins mPins;
};


#endif /* PINLAYER_H_ */
