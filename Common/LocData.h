/*
 * LocData.h
 *
 *  Created on: 11 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef LOCDATA_H_
#define LOCDATA_H_

#include <MAUtil/Environment.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <MAP/DateTime.h>
#include <MAP/LonLat.h>

struct LocData
{
	MAP::LonLat loc;
	//MAPUtil::DateTime time;
	int videoID;
	int imgID;
	int txtID;
};


#endif /* LOCDATA_H_ */
