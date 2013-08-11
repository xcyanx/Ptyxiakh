/*
 * LineLayer.cpp
 *
 *  Created on: 2 Νοε 2012
 *      Author: Σταμάτης
 */



#include "LineLayer.h"
#include <mavsprintf.h>

LineLayer::LineLayer():MapLayer()
{

}

LineLayer::~LineLayer()
{

}

void LineLayer::clearLayer()
{
	_Points.clear();

	lprintfln("Points size: %d", _Points.size());
}

void LineLayer::drawLayer(MAP::MapWidget* map)
{
	int prevColor = maSetColor(MAPUtil::Color::red.val());

	for(int i = 0; i < _Points.size()-1; i++)
	{
		MAP::PixelCoordinate pc1, pc2;
		MAPoint2d p1, p2;

		pc1 = _Points[i].toPixels(map->getMagnification());
		pc2 = _Points[i+1].toPixels(map->getMagnification());

		p1 = map->getViewport()->worldPixelToViewport(pc1);
		p2 = map->getViewport()->worldPixelToViewport(pc2);

		Gfx_pushMatrix();

			Gfx_line(p1.x, p1.y, p2.x, p2.y);

		Gfx_popMatrix();
	}

	maSetColor(prevColor);


}

void LineLayer::addPoints(MAUtil::Vector<MAP::LonLat> points)
{
	_Points = points;
}
