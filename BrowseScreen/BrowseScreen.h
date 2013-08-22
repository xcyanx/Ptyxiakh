/*
 * OptionScreen.h
 *
 *  Created on: 30 Ιουλ 2012
 *      Author: Σταμάτης
 */

#ifndef OPTIONSCREEN_H_
#define OPTIONSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
//#include <NativeUI/Widgets.h>

#include <MAUI/Screen.h>
#include <MAP/MapWidget.h>
#include <MAP/OpenStreetMapSource.h>
#include <NativeUI/Widgets.h>
#include <MAUI/Engine.h>
#include <MAP/GoogleMapSource.h>
#include <MAUI/EditBox.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Downloader.h>
#include <mactype.h>
#include <mastdlib.h>

#include "../GPS/GPSListener.h"
#include "LineLayer.h"
#include "ArrowLayer.h"
#include "PinLayer.h"
#include "Widgets/PoI.h"
#include "Common/DataStruct.h"
#include "Widgets/MapLayer.h"
#include "../PopUp/PopUpListener.h"
#include "../PopUp/PopUp.h"
#include "../Common/MAUI/Button/Button.h"
#include "../Common/MAUI/Button/ButtonListener.h"
#include "Widgets/PoI.h"
#include "Widgets/LayeredMap.h"
#include "../Common/Packets/Packet.h"
#include "../XML/XMLParser2.h"
#include "../XML/XMLParser.h"
#include "../XML/XMLListener.h"
#include <MAUtil/Environment.h>
#include "../Common/LocData.h"
//#include "../GMaps/GMap.h"


using namespace NativeUI;

class MainScreen;

class Option:  public MAUI::Screen , public GPSListener, public NativeUI::EditBoxListener, public PopUpListener,
			   public MAUI::ButtonListener, public MAUtil::ConnectionListener, public XMLListener,
			   public MAUtil::DownloadListener
{
	public:
		Option(MainScreen *scr);
		~Option();
		virtual void show();
		virtual void hide();
		MAUtil::Vector<MAP::LonLat>* getLonLatArray();
		bool isVisible();
		PinLayer *getPinLayer();
		LineLayer *getLineLayer();

	private:
		//virtual void checkBoxStateChanged(CheckBox *checkBox, bool state);
		virtual void hasMoved(GPS* gpsWidget, MALocation loc, MALocation old_loc);
		virtual void firstPoint(GPS* gpsWidget, MALocation loc);
		virtual void compassOrientationChanged(GPS* gpsWidget, int azimuth);
		virtual void editBoxReturn(NativeUI::EditBox *editBox){ };
		virtual void editBoxEditingDidBegin(NativeUI::EditBox *editBox);
		virtual void itemSelected(PopUp* obj, int index);
		virtual void buttonPressed(MAUI::Button *object);
		void pointerPressEvent(MAPoint2d p);
		void pointerMoveEvent(MAPoint2d p);
		void pointerReleaseEvent(MAPoint2d p);
		void connectFinished(MAUtil::Connection *conn, int result);
		void connWriteFinished(MAUtil::Connection *conn, int result);
		void connReadFinished(MAUtil::Connection *conn, int result);
		void XMLParsed(XMLBase *obj);
		void XMLData(XMLBase *obj, MAUtil::String &Data);
		bool outOfMemory(MAUtil::Downloader *downloader);
		void finishedDownloading(MAUtil::Downloader *downloader, MAHandle data);
		void downloadCancelled(MAUtil::Downloader *downloader);
		void error(MAUtil::Downloader *downloader, int code);

	private:
		MAUI::Button *_Button;
		MAUI::Layout *_Layout;
		MainScreen *_MScr;
		PopUp *_PopUp;
		PopUp *_RouteList;
	    bool visible;
		LayeredMap *_Map;
		//MAP::MapWidget *_Map;
		MAP::MapSource *_Src;
		XML2Parser	*_XMLParser;
		XMLParser	*_XMLParser1;
		ArrowLayer *_ArrowLayer;
		PinLayer *_PinLayer;
		LineLayer *_LineLayer;
		MAUI::Font *_Font;
		int DoubleClickTimeMs;
		int pointerPressTimeMs;
		BasicPacket *packet;
		MAUI::Image *_ZoomIn;
		MAUI::Image *_ZoomOut;
		//MAUtil::Vector<LocData> _LocData;
		MAUtil::Connection *mConnection;
		MAHandle placeholder;
		MAUtil::Vector<MAP::LonLat> loc_data;
		MAUtil::Downloader _Downloader;
		bool connected;
		bool tracking;
		MAP::LonLat _Point;
		int index;
		short writeOption;
		PoI *_PoI;
		MAUtil::Vector<int> _RouteIDs;
		//bool operatable;
};


#endif /* OPTIONSCREEN_H_ */
