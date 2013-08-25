/*
 * XMLParser2.cpp
 *
 *  Created on: 19 Νοε 2012
 *      Author: Σταμάτης
 */


#ifndef XMLPARSER_CPP_
#define XMLPARSER_CPP_

#include "XMLParser2.h"
#include "XMLListener.h"

#include <mavsprintf.h>

#define LOG(words) lprintfln(words)

XML2Parser::XML2Parser()
{
	ct.init(this, this);

	reset = false;
}

XML2Parser::~XML2Parser()
{
	_Listeners.clear();
}

void XML2Parser::parse(MAUtil::String data)
{
	//Context ct;
	_Data = data;

	buffer.clear();
	mState.clear();
	reset = false;

	ct.feedProcess(_Data.pointer());
}

void XML2Parser::mtxTagStart(const char* name, int len)
{
	if(!strcmp(name, "ROUTES"))
	{
		mState.push(ROUTES);
	}
	else if(!strcmp(name, "NAME"))
	{
		mState.push(NAME);
	}
}

void XML2Parser::mtxTagData(const char* data, int len)
{
	if(reset)
	{
		buffer.clear();

		reset = false;
	}

	buffer.append(data, len);
}

void XML2Parser::mtxTagEnd(const char* name, int len)
{
	int mState = this->mState.peek();

	this->mState.pop();

	reset = true;

	switch(mState)
	{
		case ROUTES:
		{
			for(int i = 0; i < _Listeners.size(); i++)
			{
				_Listeners[i]->XMLParsed(this);
			}

			break;
		}
		case NAME:
		{
			//create a new point and add it to the list
			for(int i = 0; i < _Listeners.size(); i++)
			{
				_Listeners[i]->XMLData(this, buffer);
			}

			break;
		}
		default:
			break;
	}
}

void XML2Parser::mtxTagAttr(const char* tagAttr, const char* something)
{
	MAUtil::String tmp(something);

	if(!strcmp(tagAttr, "RID"))
		for(int i = 0; i < _Listeners.size(); i++)
		{
			_Listeners[i]->XMLAttr(this, tmp);
		}
}

void XML2Parser::mtxParseError(int error)
{
	maPanic(0, "Error in xml.");
}

void XML2Parser::addXmlListener(XMLListener *listener)
{
	NativeUI::addListenerToVector(_Listeners, listener);
}

void XML2Parser::removeXmlListener(XMLListener *listener)
{
	NativeUI::removeListenerFromVector(_Listeners, listener);
}

#endif /* XMLPARSER_CPP_ */
