/*
 * ArrowLayer.h
 *
 *  Created on: 24 Οκτ 2012
 *      Author: Σταμάτης
 */

#ifndef ARROWLAYER_H_
#define ARROWLAYER_H_

#include "Widgets/MapLayer.h"
#include "MAHeaders.h"

#include <MAUtil/Graphics.h>
#include <maapi.h>
#include <MAUI/Image.h>

class ArrowLayer: public MapLayer
{
	public:
	    ~ArrowLayer();
		ArrowLayer();
		void drawLayer(MAP::MapWidget* map);
		void setAnimState(int state);
		int  getAnimState();
	private:
		int animState;
		MAUI::Image* animation;
};

#endif /* ARROWLAYER_H_ */
