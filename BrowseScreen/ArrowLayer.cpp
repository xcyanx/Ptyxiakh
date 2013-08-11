/*
 * ArrowLayer.cpp
 *
 *  Created on: 24 Οκτ 2012
 *      Author: Σταμάτης
 */



#include "ArrowLayer.h"

ArrowLayer::ArrowLayer(): MapLayer()
{
	MAExtent imageSize = maGetImageSize(ARROW_ANIMATION);

	int height = EXTENT_Y(imageSize);
	int width = EXTENT_X(imageSize);

	animation = new MAUI::Image(0, 0, width, height, NULL, true, true, ARROW_ANIMATION);

	animState = ARROW_ANIMATION;
}

ArrowLayer::~ArrowLayer()
{
	delete animation;
}

void ArrowLayer::drawLayer(MAP::MapWidget* map)
{
	Gfx_pushMatrix();

	Gfx_translate(map->getWidth()/2 - animation->getWidth()/2, map->getHeight()/2 - animation->getHeight()/2);

	animation->setEnabled(true);
	animation->draw(true);
	animation->setEnabled(false);

	Gfx_popMatrix();
}

void ArrowLayer::setAnimState(int state)
{
	if(state == 0)
	{
		animState = ARROW_ANIMATION;
	}
	else if(state == 1)
	{
		animState = END_OF_ANIMATION;
	}
	else
	{
		animState = ARROW_ANIMATION + state % (END_OF_ANIMATION+1);
	}

	animation->setResource(animState);
}

int ArrowLayer::getAnimState()
{
	return animState;
}
