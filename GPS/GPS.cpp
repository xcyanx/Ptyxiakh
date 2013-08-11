/*
 * GPS.cpp
 *
 *  Created on: 23 Αυγ 2012
 *      Author: Σταμάτης
 */


#include "GPS.h"
#include "GPSListener.h"
//#include <Facebook/LOG.h>

#include <mavsprintf.h>

#define LOG(words) lprintfln(words)

GPS::GPS()
{
	active = false;
	errorCode = -1;

	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
	MAUtil::Environment::getEnvironment().addSensorListener(this);

	loc.horzAcc = -1;
}

GPS::~GPS()
{
	maLocationStop();
	maSensorStop(SENSOR_TYPE_COMPASS);

	MAUtil::Environment::getEnvironment().removeCustomEventListener(this);
	MAUtil::Environment::getEnvironment().removeSensorListener(this);

	mGPSListeners.clear();
}

void GPS::gpsActivate()
{
	int res = maLocationStart();

	int res2 = maSensorStart(SENSOR_TYPE_COMPASS, 400);

	lprintfln("Compass Sensor: %d\n", res2);
	lprintfln("GPS Sensor: %d\n", res);

	loc.horzAcc = -1;

	if(res == MA_LPS_AVAILABLE || res == 0)
	{
		errorCode = res;
		active = true;
	}
	else if(res == MA_LPS_OUT_OF_SERVICE || res < 0)
	{
		errorCode = res;
		active = false;
	}
}

void GPS::gpsStop()
{
	maLocationStop();
	maSensorStop(SENSOR_TYPE_COMPASS);
	active = false;
}

void GPS::customEvent(const MAEvent &event)
{
	if(event.type == EVENT_TYPE_LOCATION)
	{
		gpsHandler(event);
	}
}

void GPS::sensorEvent(MASensor a)
{
	if(a.type == SENSOR_TYPE_COMPASS)
	{
		compassHandler(a);
	}
}


void GPS::compassHandler( MASensor &sensor)
{
	//add new event to gps handler like compassOrientChanged(...).
	//lprintfln("Compass: Azimuth: %d\n", (int)sensor.values[0]);

	for(int i = 0; i < mGPSListeners.size(); i++)
	{
		mGPSListeners[i]->compassOrientationChanged(this, (int)sensor.values[0]);
	}
}

void GPS::setAccError(double err)
{
	accError = err;
}

double GPS::getAccError()
{
	return accError;
}

void GPS::gpsHandler(const MAEvent &event)
{
	MALocation &loc = *(MALocation*)event.data;

	//maAlert("IN_GPS_HANDLER", "Test", "Ok", NULL, NULL);
	if(loc.state != MA_LOC_QUALIFIED)
	{
		//maAlert("MA_LOC_NOT_QUALIFIED", "Test", "Ok", NULL, NULL);
		LOG("GPS Log: MA_LOC_NOT_QUALIFIED\n");
		return;
	}

	//lprintfln("GPS Log: loc.lat-loc.lon, accuracy: %f-%f, %f\n", loc.lat, loc.lon, loc.horzAcc);

	for(int i = 0; i < mGPSListeners.size(); i++)
	{
		mGPSListeners[i]->gpsDataReceived(this, loc);
	}

	if(loc.horzAcc >= accError)
	{
		return;
	}

	if(this->loc.horzAcc == -1)
	{
		this->loc = loc;
		center = loc;

		for(int i = 0; i < mGPSListeners.size(); i++)
		{
			mGPSListeners[i]->firstPoint(this, this->loc);
		}

		//LOG("POINT_INIT");
		return;
	}

	if(isInCircle(loc, center, this->loc.horzAcc) && loc.horzAcc < this->loc.horzAcc)
	{
		this->loc = loc;
	}
	else if(!isInCircle(loc, center, this->loc.horzAcc))
	{
		for(int i = 0; i < mGPSListeners.size(); i++)
		{
			mGPSListeners[i]->hasMoved(this, loc, this->loc);
		}

		//this->loc = loc;
	}

	if(!mGPSListeners.empty())
		mGPSListeners[0]->gpsEnd(this, loc, this->loc);
}

void GPS::addGPSListener(GPSListener* listener)
{
	NativeUI::addListenerToVector(mGPSListeners, listener);
}

void GPS::removeGPSListener(GPSListener* listener)
{
	NativeUI::removeListenerFromVector(mGPSListeners, listener);
}

bool GPS::isActive()
{
	return active;
}

int GPS::getErrorCode()
{
	return errorCode;
}

bool GPS::isInCircle(MALocation loc1, MALocation loc2, double radius)
{
	//double x = loc2.lat - loc1.lat;
	//double y = loc2.lon - loc1.lon;
	double distance;

	distance = GPS::coordsToMeters(loc1, loc2);

	//distance = sqrt((x*x)+(y*y));

	//lprintfln("GPS->isInCircle(): distance: %f, radius: %f", distance, radius);

	if(distance <= radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double GPS::coordsToMeters(MALocation loc1, MALocation loc2)
{
	int R = 6371; //in km
	double dLat = degToRad(loc2.lat-loc1.lat);
	double dLon = degToRad(loc2.lon-loc1.lon);

	double lat1 = degToRad(loc1.lat);
	double lat2 = degToRad(loc2.lat);

	double a = sin(dLat/2)*sin(dLat/2)+sin(dLon/2)*sin(dLon/2)*cos(lat1)*cos(lat2);
	double c = 2*atan2(sqrt(a), sqrt(1-a));

	return R*c*1000;
}

double GPS::degToRad(double deg)
{
	return deg*3.14159265/180;
}

void GPS::changeCoords(MALocation loc)
{
	this->loc = loc;
	center = loc;
}

MAUtil::String GPS::getFacing(MALocation basePoint, MALocation destPoint)
{
	//lat = y, lon = x

	int angle = getAngle(basePoint, destPoint);

	//Here we check if we are close to the xx' axis.
	if(angle > -20 && angle < 20)
	{
		if(destPoint.lon > basePoint.lon)
		{
			return "E"; //East
		}
		else
		{
			return "W"; //West
		}
	}

	//Now we will check for the yy' axis;
	if((angle > 75 && angle < 90) || (angle < -75 && angle > -90))
	{
		if(destPoint.lat > basePoint.lat)
		{
			return "N";
		}
		else
		{
			return "S";
		}
	}

	//Since we are not close to any of the axis...
	//T1 or T3
	if(angle >= 0)
	{
		if(destPoint.lat > basePoint.lat)
		{
			return "NE";
		}
		else
		{
			return "SW";
		}
	}

	//T2 or T4
	if(angle < 0)
	{
		if(destPoint.lat > basePoint.lat)
		{
			return "NW";
		}
		else
		{
			return "SE";
		}
	}
}

bool GPS::doubleEqu(double val1, double val2)
{
	double epsilon = 0.000000001;
	return fabs(val1 - val2) < epsilon;
}

int GPS::getAngle(MALocation val1, MALocation val2)
{
	double l;
	double result;

	if(val2.lon-val1.lon)
	{
		if(doubleEqu(sin(val2.lat-val1.lat), 1))
		{
			return 90;
		}
		else
		{
			return -90;
		}
	}

	l = (val2.lat-val1.lat)/(val2.lon-val1.lon);

	result = atan(l);

	return (int)(result * 180 / 3.14159265);
}
