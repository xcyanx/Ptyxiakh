/*
 * XML.h
 *
 *  Created on: 26 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef XML_H_
#define XML_H_

#include <MAUtil/Environment.h>
#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/FileLister.h>
#include <MAP/DateTime.h>
//#include <MAFS/File.h>
#include "../Common/DataStruct.h"


class XML
{
	public:
		XML(MAUtil::String filename);
		~XML();
		void WriteNode(Data data);
		void finalize();
		int getSize();
		MAHandle getHandle();


	private:
		void CreateRoot();
		int getRemaining(MAHandle file);
		void CloseRoot();
		bool writeDataToXML(MAUtil::String filename);
		int getSystemProperty(const char* key, MAUtil::String& dst);
		MAUtil::String getLocalPath();
		MAUtil::String dateToString(MAPUtil::DateTime time);

	private:
		MAHandle file;
};


#endif /* XML_H_ */
