/*
 * XML.cpp
 *
 *  Created on: 26 Αυγ 2012
 *      Author: Σταμάτης
 */




#include "XML.h"
#include <mavsprintf.h>


int XML::getSystemProperty(const char* key, MAUtil::String& dst)
{
	    int size = maGetSystemProperty(key, NULL, 0);
	    if(size < 0)
	        return size;
	    dst.resize(size-1);
	    maGetSystemProperty(key, dst.pointer(), size);
	    return size;
}

MAUtil::String XML::getLocalPath()
{
	    // Do this here to work around a MoRE bug.
	    FileLister fl;
	    fl.start("/");

	    MAUtil::String path, os;


	    // Try getting the local path.
	    int result = getSystemProperty("mosync.path.local", path);

	    getSystemProperty("mosync.device.OS", os);

	    lprintfln("OS: %s", MAUtil::lowerString(os).c_str());

	    if(MAUtil::lowerString(os).find("android", 0) != -1)
	    {
	    	MAHandle tfile;

	    	path = "/sdcard/Magna Carta/";

	    	tfile = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);

	    	lprintfln("File handle: %d", tfile);
	    	lprintfln("File exists: %d", maFileExists(tfile));


	    	if(!maFileExists(tfile))
	    	{
	    		int result = maFileCreate(tfile);

	    		lprintfln("File create code: %d", result);
	    	}

	    	maFileClose(tfile);

	    	lprintfln("Path: %s", path.c_str());

	    	return path;
	    }

	    // If it works, fine.
	    if(result > 0) {
	        //printf("Got local path: %i\n", result);
	        return path;
	    }

	    // Otherwise, get the first root directory.
	    fl.start("");
	    result = fl.next(path);
	    //MAASSERT(result > 0);
	    return path;
}

int XML::getSize()
{
	return maFileSize(XML::file);
}

MAHandle XML::getHandle()
{
	return XML::file;
}

XML::XML(MAUtil::String filename)
{
	MAUtil::String file = getLocalPath()+filename;

	file += ".xml";

	XML::file = maFileOpen(file.c_str(), MA_ACCESS_READ_WRITE);

	int res = maFileExists(XML::file);

	if(res == 1)
	{
		maFileTruncate(XML::file, 0);
	}
	else if(res == 0)
	{
		maFileCreate(XML::file);
	}
	else
	{
		maPanic(0, "Error on file creation.");
	}

	CreateRoot();

	_ActivityIndicator = new WaitMessage("Please, wait...", "Saving data to file.");
}

void XML::CreateRoot()
{
	MAUtil::String root("<root>");
	maFileWrite(file, root.c_str(), root.size());
}

void XML::CloseRoot()
{
	MAUtil::String root("</root>");
	maFileWrite(file, root.c_str(), root.size());
}

bool XML::writeDataToXML(MAUtil::String filename)
{
	MAHandle tfile;
	int dataLen = 256; //we set the dataLen to a fixed size for now.
	byte *data;
	//int fileSize;

	tfile = maFileOpen(filename.c_str(), MA_ACCESS_READ_WRITE);

	if(!maFileExists(tfile))
	{
		lprintfln("File does not exist: %s", filename.c_str());
		return false;
	}


	data = new byte[dataLen];

	while(getRemaining(tfile))
	{
		int rwResult;

		if(getRemaining(tfile) < 256)
		{
			dataLen = getRemaining(tfile);
		}

		rwResult = maFileRead(tfile, data, dataLen);
		rwResult += maFileWrite(file, data, dataLen);

		if(rwResult != 0)
		{
			return false;
		}

		memset(data, 0 , 256);
	}

	delete data;

	maFileClose(tfile);

	return true;
}

int XML::getRemaining(MAHandle file)
{
	return maFileSize(file) - maFileTell(file);
}

void XML::finalize()
{
	this->CloseRoot();
}

void XML::WriteNode(Data data)
{

	MAUtil::String coords_tag = "<Point>\n";

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	coords_tag = "	<Coords>\n";

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	//Write the Lan and Lon values.
	coords_tag = "		<Lat>";
	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	MAUtil::String data2;
	data2 = MAUtil::doubleToString(data.loc.lat);

	maFileWrite(file, data2.c_str(), data2.size());

	coords_tag = "</Lat>\n";
	maFileWrite(file, coords_tag.c_str(), coords_tag.size());


	coords_tag = "		<Lon>";
	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	data2 = MAUtil::doubleToString(data.loc.lon);

	maFileWrite(file, data2.c_str(), data2.size());

	coords_tag = "</Lon>\n";
	maFileWrite(file, coords_tag.c_str(), coords_tag.size());
	//
	coords_tag = "	</Coords>\n";

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());


	if(data.imagePath.size() > 0)
	{
		_ActivityIndicator->show();

		byte *tdata;
		int dataLen;
		MAUtil::String img_tag = "	<Image>";


		lprintfln("Image file: %s", data.imagePath.c_str());

		maFileWrite(file, img_tag.c_str(), img_tag.size());
		//maFileWrite(file, data.imagePath.c_str(), data.imagePath.size());
		//Write the data to the xml

		if(!writeDataToXML(data.imagePath))
		{
			maPanic(0, "Couldn't locate the image file.");
		}

		//maFileWrite(file, tdata, dataLen);
		//End of data writting.

		img_tag = "</Image>\n";

		maFileWrite(file, img_tag.c_str(), img_tag.size());

		_ActivityIndicator->hide();
	}


	if(data.videoPath.size() > 0)
	{
		_ActivityIndicator->show();

		byte *tdata;
		int dataLen;
		MAUtil::String vid_tag = "	<Video>";

		maFileWrite(file, vid_tag.c_str(), vid_tag.size());
		//maFileWrite(file, data.videoPath.c_str(), data.videoPath.size());
		//Write the data to the xml

		lprintfln("Image file: %s", data.videoPath.c_str());

		if(!writeDataToXML(data.videoPath))
		{
			maPanic(0, "Couldn't locate the video file.");
		}

		//maFileWrite(file, tdata, dataLen);
		//End of data writting.

		vid_tag = "</Video>\n";

		maFileWrite(file, vid_tag.c_str(), vid_tag.size());

		_ActivityIndicator->hide();
	}

	if(data.text.size() > 0)
	{
		MAUtil::String text_tag = "	<Text>";

		maFileWrite(file, text_tag.c_str(), text_tag.size());
		maFileWrite(file, data.text.c_str(), data.text.size());

		text_tag = "</Text>\n";

		maFileWrite(file, text_tag.c_str(), text_tag.size());
	}


	//Write the date time.
	coords_tag = "	<DateTime>";

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	coords_tag = dateToString(data.time);

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	coords_tag = "</DateTime>\n";

	maFileWrite(file, coords_tag.c_str(), coords_tag.size());

	//End of date time.

	coords_tag = "</Point>";
	maFileWrite(file, coords_tag.c_str(), coords_tag.size());
}


MAUtil::String XML::dateToString(MAPUtil::DateTime time)
{
	MAUtil::String result;

	result = MAUtil::integerToString(time.getDay());
	result += "/";
	result += MAUtil::integerToString(time.getMonth()+1);
	result += "/";
	result += MAUtil::integerToString(time.getYear()-100);
	result += " ";
	result += MAUtil::integerToString(time.getHour());
	result += ":";
	result += MAUtil::integerToString(time.getMinute());
	result += ":";
	result += MAUtil::integerToString(time.getSecond());

	return result;
}

XML::~XML()
{
	CloseRoot();
	maFileClose(file);

	delete _ActivityIndicator;
}
