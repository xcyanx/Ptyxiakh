/*
 * GPS.h
 *
 *  Created on: 23 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef GPS_H_
#define GPS_H_

#include <MAUtil/Environment.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <madmath.h>

namespace NativeUI
{
	class GPSListener;

	class GPS: public MAUtil::CustomEventListener, public MAUtil::SensorListener
	{
		public:
			GPS();
			~GPS();
			bool isActive();
			void gpsActivate();
			void gpsStop();
			int getErrorCode();
			void setAccError(double err);
			double getAccError();
			static double coordsToMeters(MALocation loc1, MALocation loc2);
			void changeCoords(MALocation loc);
			static MAUtil::String getFacing(MALocation basePoint, MALocation destPoint);
			virtual void addGPSListener(GPSListener *listener);
			virtual void removeGPSListener(GPSListener *listener);

		private:
			virtual void customEvent(const MAEvent &event);
			virtual void sensorEvent(MASensor a);
			bool isInCircle(MALocation loc1, MALocation loc2, double radius);
			static double degToRad(double deg);
			void gpsHandler(const MAEvent &event);
			void compassHandler(MASensor &sensor);
			static bool doubleEqu(double val1, double val2);
			static int getAngle(MALocation val1, MALocation val2);

		private:
			bool active;
			int errorCode;
			MALocation loc;
			MALocation center;
			double accError;
			MAUtil::Vector<GPSListener*> mGPSListeners;
	};
}


#endif /* GPS_H_ */
