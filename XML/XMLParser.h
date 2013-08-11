/*
 * XMLParser.h
 *
 *  Created on: 11 Σεπ 2012
 *      Author: Σταμάτης
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <MTXml/MTXml.h>
#include <MAUtil/util.h>
#include <NativeUI/Widgets.h>
#include <MAUtil/Stack.h>

#include "../Common/LocData.h"
#include "XMLBase.h"

using namespace Mtx;

class XMLListener;


class XMLParser: public XmlListener, public MtxListener, public XMLBase
{
	public:
	  XMLParser();
	  ~XMLParser();


	  void parse(MAUtil::String data);

	  //XmlListener
	  void mtxDataRemains(const char *data, int len){ };
	  void mtxTagStart(const char* name, int len);
	  void mtxTagData(const char* data, int len);
	  void mtxTagEnd(const char* name, int len);
	  void mtxParseError();
	  void addXmlListener(XMLListener *listener);
	  void removeXmlListener(XMLListener *listener);
	  void mtxEncoding(const char* encoding){ };
	  void mtxTagStartEnd(){ };
	  void mtxEmptyTagEnd(){ };
	  void mtxTagAttr(const char* tagAttr, const char* something) { };
	  void mtxParseError(int error);
	  const MAUtil::Vector<LocData>& getArray();
	  //unsigned char mtxUnicodeCharacter(int unicode);

	private:
	  MAUtil::String buffer;
	  enum XmlTags
	  	{
		  LOCATION,
		  POINT,
	  	  LATITUDE,
	  	  LONGITUDE,
	  	  VIDID,
	  	  IMGID,
	  	  TEXTID
	  	};

	  	MAUtil::Stack<XmlTags> mState;
	  	bool reset;
	  	Context ct;
	  	MAUtil::String _Data;
	  	MAUtil::Vector<LocData> _LocDataArray;
	  	MAUtil::Vector<XMLListener*> _Listeners;
};

#endif /* XMLPARSER_H_ */
