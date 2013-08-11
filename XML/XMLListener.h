/*
 * XMLListener.h
 *
 *  Created on: 11 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef XMLLISTENER_H_
#define XMLLISTENER_H_

class XMLParser;
class XMLBase;
class LocData;

class XMLListener
{
	public:
		virtual void XMLParsed(XMLBase *obj)=0;
		virtual void XMLLocData(XMLBase *obj, LocData &Data) { };
		virtual void XMLData(XMLBase *obj, MAUtil::String &Data){ };
};


#endif /* XMLLISTENER_H_ */
