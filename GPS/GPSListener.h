/*
 * GPSListener.h
 *
 *  Created on: 23 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef GPSLISTENER_H_
#define GPSLISTENER_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

namespace NativeUI
{
	class GPS;

	class GPSListener
	{
		public:
			virtual void gpsDataReceived(GPS* gpsWidget, MALocation loc);
			virtual void hasMoved(GPS* gpsWidget, MALocation loc, MALocation old_loc)=0;
			virtual void firstPoint(GPS* gpsWidget, MALocation loc)=0;
			virtual void compassOrientationChanged(GPS* gpsWidget, int azimuth)=0;
			virtual void gpsEnd(GPS* gpsWidget, MALocation loc, MALocation old_loc);
	};
}


#endif /* GPSLISTENER_H_ */
