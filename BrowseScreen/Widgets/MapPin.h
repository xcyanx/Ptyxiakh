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

#ifndef MAPPIN_H_
#define MAPPIN_H_

#include <MAP/LonLat.h>
#include <MAUI/Widget.h>

//using namespace MAP;
//using namespace MAUI;

//A base class which you can use to place widgets on the map

namespace MAUI
{

class MapPin : public Widget
{
	public:
		MapPin(int x, int y, int width, int height, Widget* parent);
		~MapPin();

		void setLonLat(MAP::LonLat& lonlat);
		void setLonLat(double lon, double lat);
		void setDataIds(int img, int vid, int txt);
		int getTxtID();
		int getVidID();
		int getImgID();

		MAP::LonLat& getLonLat();

        virtual int getXOffset() = 0;
        virtual int getYOffset() = 0;

	private:
		MAP::LonLat mLL;
		int txtID;
		int vidID;
		int imgID;
};

}
#endif /* MAPPIN_H_ */
