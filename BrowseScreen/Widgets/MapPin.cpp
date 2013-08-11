/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "MapPin.h"
#include <MAUtil/Graphics.h>
#include <MAUI/Label.h>
//#include "../std.h"

MAUI::MapPin::MapPin(int x, int y, int width, int height, Widget* parent)
	  : Widget(x, y, width, height, parent)
{
	txtID = 0;
	vidID = 0;
	imgID = 0;
}

MAUI::MapPin::~MapPin()
{}

void MAUI::MapPin::setLonLat(MAP::LonLat& ll)
{
	mLL.lat = ll.lat;
	mLL.lon = ll.lon;
}

void MAUI::MapPin::setDataIds(int img, int vid, int txt)
{
	txtID = txt;
	vidID = vid;
	imgID = img;
}

void MAUI::MapPin::setLonLat(double lon, double lat)
{
	mLL.lat = lat;
	mLL.lon = lon;
}

int MAUI::MapPin::getTxtID()
{
	return txtID;
}

int MAUI::MapPin::getVidID()
{
	return vidID;
}

int MAUI::MapPin::getImgID()
{
	return imgID;
}

MAP::LonLat& MAUI::MapPin::getLonLat()
{
	return mLL;
}
