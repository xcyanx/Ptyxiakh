/*
 * XMLParser.cpp
 *
 *  Created on: 11 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef XMLPARSER_CPP_
#define XMLPARSER_CPP_

#include "XMLParser.h"
#include "XMLListener.h"

#include <mavsprintf.h>

#define LOG(words) lprintfln(words)

XMLParser::XMLParser()
{
	ct.init(this, this);

	reset = false;
}

void XMLParser::parse(MAUtil::String data)
{
	_Data = data;

	_LocDataArray.clear();
	mState.clear();
	reset = false;

	ct.feedProcess(_Data.pointer());
}

XMLParser::~XMLParser()
{
	_Listeners.clear();
}

void XMLParser::mtxTagStart(const char* name, int len)
{
	if(!strcmp(name, "locations"))
	{
		mState.push(LOCATION);
	}
	else if(!strcmp(name, "point"))
	{
		mState.push(POINT);

		//create a new point and add it to the list
		LocData data;

		data.imgID = 0;
		data.txtID = 0;
		data.videoID = 0;

		_LocDataArray.add(data);
	}
	else if(!strcmp(name, "Lat"))
	{
		mState.push(LATITUDE);
	}
	else if(!strcmp(name, "Lon"))
	{
		mState.push(LONGITUDE);
	}
	else if(!strcmp(name, "VidID"))
	{
		mState.push(VIDID);
	}
	else if(!strcmp(name, "ImgID"))
	{
		mState.push(IMGID);
	}
	else if(!strcmp(name, "TextID"))
	{
		mState.push(TEXTID);
	}
}

void XMLParser::mtxTagData(const char* data, int len)
{
	if(reset)
	{
		buffer.clear();

		reset = false;
	}

	buffer.append(data, len);
}

void XMLParser::mtxTagEnd(const char* name, int len)
{
	int mState = this->mState.peek();

	this->mState.pop();

	reset = true;

	switch(mState)
	{
		case LOCATION:
		{
			for(int i = 0; i < _Listeners.size(); i++)
			{
				_Listeners[i]->XMLParsed(this);
			}

			for(int i = 0; i < _LocDataArray.size(); i++)
			{
				lprintfln("Lon2 from parser: %lf\n", _LocDataArray[i].loc.lon);
				lprintfln("Lat2 from parser: %lf\n", _LocDataArray[i].loc.lat);
			}

			break;
		}
		case POINT:
		{
			break;
		}
		case LONGITUDE:
		{
			_LocDataArray[_LocDataArray.size()-1].loc.lon = MAUtil::stringToDouble(buffer);

			lprintfln("Lon from parser: %s\n", buffer.c_str());
			break;
		}
		case LATITUDE:
		{
			_LocDataArray[_LocDataArray.size()-1].loc.lat = MAUtil::stringToDouble(buffer);

			lprintfln("Lat from parser: %s\n", buffer.c_str());
			break;
		}
		case VIDID:
		{
			_LocDataArray[_LocDataArray.size()-1].videoID = MAUtil::stringToInteger(buffer);

			break;
		}
		case IMGID:
		{
			_LocDataArray[_LocDataArray.size()-1].imgID = MAUtil::stringToInteger(buffer);

			break;
		}
		case TEXTID:
		{
			_LocDataArray[_LocDataArray.size()-1].txtID = MAUtil::stringToInteger(buffer);

			break;
		}
		default:
			break;
	}
}

void XMLParser::mtxParseError(int error)
{
	maPanic(0, "Error in xml.");
}

void XMLParser::addXmlListener(XMLListener *listener)
{
	NativeUI::addListenerToVector(_Listeners, listener);
}

void XMLParser::removeXmlListener(XMLListener *listener)
{
	NativeUI::removeListenerFromVector(_Listeners, listener);
}

const MAUtil::Vector<LocData>& XMLParser::getArray()
{
	return _LocDataArray;
}

#endif /* XMLPARSER_CPP_ */
