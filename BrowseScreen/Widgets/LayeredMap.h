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

#ifndef SUPERMAPWIDGET_H_
#define SUPERMAPWIDGET_H_
#include <MAP/MapWidget.h>
#include <MAP/LonLat.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Graphics.h>
#include "MapLayer.h"

//using namespace MAP;
//using namespace MAUI;
//using namespace MAUtil;

class LayeredMap : public MAP::MapWidget
{
  public:
	LayeredMap(int x, int y, int width, int height, MAUI::Widget* parent = NULL);
    ~LayeredMap();

    void drawOverlay();

    void updateMap();
    void addLayer(MapLayer* layer);
    MAUtil::Vector<MapLayer*> getLayers();

  private:
    MAUtil::Vector<MapLayer*> mLayers;
};

#endif /* SUPERMAPWIDGET_H_ */
