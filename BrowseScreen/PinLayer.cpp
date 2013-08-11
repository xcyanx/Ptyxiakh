/*
 * PinLayer.cpp
 *
 *  Created on: 2 Νοε 2012
 *      Author: Σταμάτης
 */



#include "PinLayer.h"
#include "Widgets/PoI.h"

PinLayer::PinLayer(): MapLayer()
{

}

PinLayer::~PinLayer()
{
	Vector_each(MAUI::MapPin*, itr, mPins)
		delete *itr;
}

void PinLayer::activate(bool activate)
{
	if(activate)
	{
		for(int i = 0; i < mPins.size(); i++)
		{
			((PoI*)mPins[i])->disable(!activate);
		}
	}
}

PinLayer::Pins PinLayer::getPins()
{
	return mPins;
}

void PinLayer::clearLayer()
{
	for(int i = 0; i < mPins.size(); i++)
	{
		delete mPins[i];
	}

	mPins.clear();
}

void PinLayer::drawLayer(MAP::MapWidget* map)
{
	//LOG("Map Layer being drawn");
	if(mVisible)
	{
		//LOG("Drawing MapPins");

		MAUtil::Vector<MAUI::MapPin*>::iterator itr;

		Vector_each(MAUI::MapPin*, itr, mPins)
		{
			MAUI::MapPin* mp = *itr;
			//Get the position of this point in pixels
			//Get the world pixels
			MAP::PixelCoordinate pc = mp->getLonLat().toPixels(map->getMagnification());
			//LOG("World pixels %d : %d", pc.getX(), pc.getY());
			MAPoint2d point = map->getViewport()->worldPixelToViewport(pc);
			//LOG("Pixel position is %d : %d", point.x, point.y);

			Gfx_pushMatrix();

				//Transform to the position on screen
				Gfx_translate(point.x + mp->getXOffset(), point.y + mp->getYOffset());

				//Draw the widget as normal
				(*itr)->setEnabled(true);
				(*itr)->draw(true);
				(*itr)->setEnabled(false);

			Gfx_popMatrix();
		}

	}
}

void PinLayer::addPin(MAUI::MapPin* pin)
{
	mPins.add(pin);
}
