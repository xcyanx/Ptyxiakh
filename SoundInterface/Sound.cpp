/*
 * Sound.cpp
 *
 *  Created on: 31 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef SOUND_CPP_
#define SOUND_CPP_

#include "Sound.h"

void Sound::PlaySound(MAHandle sound)
{
	if(maSoundIsPlaying())
	{
		return;
	}
	maSoundPlay(sound, 0, maGetDataSize(sound));
}

#endif /* SOUND_CPP_ */
