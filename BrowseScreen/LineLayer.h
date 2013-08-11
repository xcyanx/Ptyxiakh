/*
 * LineLayer.h
 *
 *  Created on: 2 Νοε 2012
 *      Author: Σταμάτης
 */

#ifndef LINELAYER_H_
#define LINELAYER_H_

#include "Widgets/MapLayer.h"
#include "MAHeaders.h"

#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>
#include <maapi.h>
#include <MAUI/Image.h>
#include <MAP/LonLat.h>
#include "MAP/Color.h"



class LineLayer: public MapLayer
{
	public:
		~LineLayer();
		LineLayer();
		void drawLayer(MAP::MapWidget* map);
		void clearLayer();
		void addPoints(MAUtil::Vector<MAP::LonLat> points);

	private:
		MAUtil::Vector<MAP::LonLat> _Points;
};


#endif /* LINELAYER_H_ */
