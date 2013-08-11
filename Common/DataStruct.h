/*
 * DataStruct.h
 *
 *  Created on: 26 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_

#include <MAUtil/Environment.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <MAP/DateTime.h>

struct Data
{
	MALocation loc;
	MAUtil::String imagePath;
	MAPUtil::DateTime time;
	MAUtil::String videoPath;
	MAUtil::String text;
};


#endif /* DATASTRUCT_H_ */
