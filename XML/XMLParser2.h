/*
 * XMLParser2.h
 *
 *  Created on: 19 Νοε 2012
 *      Author: Σταμάτης
 */

#ifndef XMLPARSER2_H_
#define XMLPARSER2_H_

#include <MTXml/MTXml.h>
#include <MAUtil/util.h>
#include <MAUtil/Stack.h>
#include <NativeUI/Widgets.h>

#include "../Common/LocData.h"
#include "XMLBase.h"

using namespace Mtx;

class XMLListener;

class XML2Parser: public XmlListener, public MtxListener, public XMLBase
{
	public:
	  XML2Parser();
	  ~XML2Parser();

	  void parse(MAUtil::String data);

	  //XmlListener
	  void mtxDataRemains(const char *data, int len){ };
	  void mtxTagStart(const char* name, int len);
	  void mtxTagData(const char* data, int len);
	  void mtxTagEnd(const char* name, int len);
	  void mtxParseError(int error);
	  void addXmlListener(XMLListener *listener);
	  void removeXmlListener(XMLListener *listener);
	  void mtxEncoding(const char* encoding){ };
	  void mtxTagStartEnd(){ };
	  void mtxEmptyTagEnd(){ };
	  void mtxTagAttr(const char* tagAttr, const char* something) { };
	  //const MAUtil::Vector<LocData> getArray();
	  //unsigned char mtxUnicodeCharacter(int unicode);

	private:
	  enum XmlTags
	  	{
		  ROUTES,
		  NAME
	  	};

	  	MAUtil::Stack<XmlTags> mState;
	  	MAUtil::String buffer;
	  	bool reset;
	  	Context ct;
	  	MAUtil::String _Data;
	  	//MAUtil::Vector<LocData> _LocDataArray;
	  	MAUtil::Vector<XMLListener*> _Listeners;
};


#endif /* XMLPARSER2_H_ */
