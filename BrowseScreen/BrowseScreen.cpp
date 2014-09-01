/*
 * OptionScreen.cpp
 *
 *  Created on: 30 Ιουλ 2012
 *      Author: Σταμάτης
 */


#include "BrowseScreen.h"
#include "../PreviewScreen/PreviewScreen.h"
#include "../MainScreen.h"
#include <mavsprintf.h>

#define URL "socket://192.168.1.100:3015"
#define BASEURL "http://www.example.com"

#define LOG(words) lprintfln(words)

Option::Option(MainScreen *scr):Screen(), visible(false), _MScr(scr), DoubleClickTimeMs(200),
								pointerPressTimeMs(-1), writeOption(-1)
{

	MAExtent tSize = maGetScrSize();

	int sizeX = EXTENT_X(tSize);
	int sizeY = EXTENT_Y(tSize);

    _Map = new LayeredMap(0, 0, sizeX, sizeY, NULL);


    _Src = new MAP::OpenStreetMapSource();


    _Map->setViewport(new MAP::MapViewport());
    _Map->setMagnification(17);
    _Map->setMapSource(_Src);
    _Map->setCenterPosition(MAP::LonLat(37.93445,23.615627), true, false);

    _Font = new MAUI::Font(MY_FONT);

    _Button = new MAUI::Button(0, (int)(sizeY*0.90), sizeX, (int)(sizeY*0.10), NULL);
    _Button->setCaption("Get all available routes");
    _Button->setBackgroundColor(MAP::Color::black.val());
    _Button->setFont(_Font);
    _Button->setSkin(new MAUI::WidgetSkin(BUTTON_IMG, BUTTON_IMG_INAC, 32, 362, 24, 68));


    tSize = maGetImageSize(ZOOM_IN);
    sizeX = EXTENT_X(tSize);
    sizeY = EXTENT_Y(tSize);

    _ZoomIn = new MAUI::Image(0, 0, sizeX, sizeY, NULL);
    _ZoomIn->setResource(ZOOM_IN);

    _ZoomOut = new MAUI::Image(sizeX, 0, sizeX+1, sizeY, NULL);
    _ZoomOut->setResource(ZOOM_OUT);


    _ArrowLayer = new ArrowLayer();
    _PinLayer = new PinLayer();
    _LineLayer = new LineLayer();

    _Map->addLayer(_LineLayer);
    _Map->addLayer(_PinLayer);
    _Map->addLayer(_ArrowLayer);

    _Map->add(_Button);
    _Map->add(_ZoomIn);
    _Map->add(_ZoomOut);

    _PopUp = new PopUp();
    _RouteList = new PopUp();

    //_PopUp->applyItemSkin(SELECTED_IMG, UNSELECTED_IMG, 16, 32, 16, 32, true, true);
    _PopUp->setItemFont(_Font);
    _PopUp->addOption("Show Video");
    _PopUp->addOption("Show Image");
    _PopUp->addOption("Show Text");
    _PopUp->addPopUpListener(this);

    _RouteList->addPopUpListener(this);
    _RouteList->setItemFont(_Font);

    _Button->addButtonListener(this);
    //_Button->setEnabled(false);

    connected = false;
    //operatable = false;

    mConnection = new MAUtil::Connection(this);

    //mConnection->connect("socket://46.177.24.169:3015");

    _XMLParser = new XML2Parser();
    _XMLParser1 = new XMLParser();

    _XMLParser->addXmlListener(this);
    _XMLParser1->addXmlListener(this);

    _Downloader.addDownloadListener(this);

    placeholder = -1;

    tracking = true;

    index = 0;

    _gps.addGPSListener(this);

    Option::Screen::setMain(_Map);
}

void Option::itemSelected(PopUp* obj, int index)
{
	//_RouteList popup handler.

	lprintfln("Caller: %u", obj->getCaller());
	lprintfln("Index: %d", index);

	if(obj == _RouteList)//if(obj->getCaller() == NULL)
	{
		obj->hide();

		lprintfln("We are in Route popup.");

		if(index == 0)
		{
			tracking = true;

			lprintfln("Index selected is 0");

			return;
		}

		lprintfln("Index selected is: %d", index);

		tracking = false;

		loc_data.clear();

		lprintfln("And we are requesting the route data.");

		packet = new RRouteData();

		packet->PacketID = BasicPacket::DATA_REQUESTROUTEDATA;

		((RRouteData*)packet)->route_id = _RouteIDs[index];


		//mConnection->write(packet, sizeof(RRouteData));

		writeOption = BasicPacket::DATA_REQUESTROUTEDATA;

		mConnection->close();
		mConnection->connect(URL);

		//mConnection->write(packet, sizeof(NextPacketSize));

	}
	else
	{
		_PoI = (PoI*)obj->getCaller();

		switch(index)
		{
			case 0:
				//_MScr->callPreviewScreen(PreviewScreen::VideoScr);
				//packet = new VideoReqPacket();

				//((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_VIDEO_URLREQ;
				//((VideoReqPacket*)packet)->videoID = tempPoI->getVidID();

				writeOption = BasicPacket::DATA_VIDEO_URLREQ;

				//mConnection->write(packet, sizeof(VideoReqPacket));

				break;
			case 1:
				//_MScr->callPreviewScreen(PreviewScreen::ImageScr);
				//packet = new VideoReqPacket();

				//((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_IMG_URLREQ;
				//((VideoReqPacket*)packet)->videoID = tempPoI->getImgID();

				writeOption = BasicPacket::DATA_IMG_URLREQ;

				//mConnection->write(packet, sizeof(VideoReqPacket));

				break;
			case 2:
				//_MScr->callPreviewScreen(PreviewScreen::TextScr);
				//packet = new VideoReqPacket();

				//((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_TEXT_REQ;
				//((VideoReqPacket*)packet)->videoID = tempPoI->getTxtID();

				writeOption = BasicPacket::DATA_TEXT_REQ;

				//mConnection->write(packet, sizeof(VideoReqPacket));

				break;
			default:
				break;
		}

		mConnection->close();
		mConnection->connect(URL);
	}
}

void Option::buttonPressed(MAUI::Button *object)
{
	//Use the connection to download the available routes.
	lprintfln("Button pressed.");

	object->setEnabled(false);

	//packet = new BasicPacket();

	//packet->PacketID = BasicPacket::DATA_REQUESTROUTES;

	//mConnection->write(packet, sizeof(BasicPacket));

	writeOption = BasicPacket::DATA_REQUESTROUTES;

	mConnection->close();
	mConnection->connect(URL);
}

void Option::show()
{
	//_MScr->pop();
	visible = true;
	_MScr->setVisible(false);

	_Button->addButtonListener(this);
	_PinLayer->activate(true);

	Option::Screen::show();
	maWidgetScreenShow(0);

	//_Button->setEnabled(connected);
	_Button->setEnabled(true);
	_gps.gpsActivate();
}

void Option::hide()
{
	visible = false;

	//Hide the pop up.
	_PopUp->hide();

	_Button->removeButtonListener(this);

	_PinLayer->activate(false);

	Option::Screen::hide();

	_MScr->setVisible(true);
	//_MScr->getScreen()->setVisible(true);
	_MScr->show();

	_gps.gpsStop();
}

void Option::editBoxEditingDidBegin(NativeUI::EditBox *editBox)
{
	editBox->hideKeyboard();

	_MScr->setVisible(false);
	visible = true;

	//Option::Screen::show();
	maWidgetScreenShow(0);
}

bool Option::isVisible()
{
	return visible;
}

/*void Option::checkBoxStateChanged(CheckBox *checkBox, bool state)
{
	*retValue = state;
}*/

void Option::compassOrientationChanged(GPS* gpsWidget, int azimuth)
{
	if(azimuth > 360 || azimuth < 0) //something went wrong
	{
		return;
	}

	if(!this->isVisible()) //There is no map loaded.
	{
		return;
	}

	_ArrowLayer->setAnimState(azimuth);
	_Map->updateMap();

	if(tracking)
		return;

	MALocation tmp1;
	MALocation tmp2;

	tmp1.lon = _Point.lon;
	tmp1.lat = _Point.lat;

	tmp2.lon = loc_data[index].lon;
	tmp2.lat = loc_data[index].lat;

	MAUtil::String currentAzimuth = gpsWidget->getFacing(tmp1, tmp2);

	/* 	Now if a map is loaded check
	 * 	to see if we are facing
	 * 	at the right direction.
	 */

	if(azimuth > 340 || azimuth < 20) //We are facing north
	{
		if(currentAzimuth != "N")
		{
			Sound::PlaySound(NORTH);
		}
	}
	else if(azimuth > 70 && azimuth < 110) //East
	{
		if(currentAzimuth != "E")
		{
			Sound::PlaySound(EAST);
		}
	}
	else if(azimuth > 160 && azimuth < 180) //South
	{
		if(currentAzimuth != "S")
		{
			Sound::PlaySound(SOUTH);
		}
	}
	else if(azimuth > 250 && azimuth < 290) //West
	{
		if(currentAzimuth != "W")
		{
			Sound::PlaySound(WEST);
		}
	}
	else if(azimuth >= 20 && azimuth <= 70) //North-East
	{
		if(currentAzimuth != "NE")
		{
			Sound::PlaySound(NORTH_EAST);
		}
	}
	else if(azimuth >= 110 && azimuth <= 160) //South-East
	{
		if(currentAzimuth != "SE")
		{
			Sound::PlaySound(SOUTH_EAST);
		}
	}
	else if(azimuth >= 180 && azimuth <= 250) //South-West
	{
		if(currentAzimuth != "SW")
		{
			Sound::PlaySound(SOUTH_WEST);
		}
	}
	else if(azimuth >= 290 && azimuth <= 340) //North-West
	{
		if(currentAzimuth != "NW")
		{
			Sound::PlaySound(NORTH_WEST);
		}
	}
}

void Option::pointerPressEvent(MAPoint2d p)
{

	if(_ZoomIn->contains(p.x, p.y))
	{
		_Map->zoomIn();

		//lprintfln("Zoom Level: %f\n", double(_Map->getViewport()->getMagnification()));
		return;
	}

	if(_ZoomOut->contains(p.x, p.y))
	{
		_Map->zoomOut();
		return;
	}

	if(_Button->contains(p.x, p.y))
	{
		return;
	}

	PinLayer::Pins Pins = _PinLayer->getPins();

	for(int i = 0; i < Pins.size(); i++)
	{
		if(Pins[i]->contains(p.x, p.y))
		{
			return;
		}
	}

	_Map->getViewport()->beginPanning(p);

	int curTimeMs = maGetMilliSecondCount( );
	int deltaTimeMs = curTimeMs - pointerPressTimeMs;

	if ( deltaTimeMs < DoubleClickTimeMs )
	{
		_Map->getViewport()->zoomIn();

		//lprintfln("Zoom Level: %f\n", double(_Map->getViewport()->getMagnification()));
	}
	else
	{
		pointerPressTimeMs = curTimeMs;
	}
}

void Option::pointerReleaseEvent(MAPoint2d p)
{
	_Map->getViewport()->endPanning();
}

void Option::pointerMoveEvent(MAPoint2d p)
{
	_Map->getViewport()->updatePanning(p);
}

void Option::connectFinished (Connection *conn, int result)
{
	if(result <= 0)
	{
		maAlert("Magna Carta", "Connection: Could not connect to server.", "Ok", NULL, NULL);

		_Button->setEnabled(true);
	}
	else
	{
		connected = true;
		//operatable = true;
	}

	switch(writeOption)
	{
		case BasicPacket::DATA_REQUESTROUTES:
		{

			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			packet = new BasicPacket();

			packet->PacketID = BasicPacket::DATA_REQUESTROUTES;

			mConnection->write(packet, sizeof(BasicPacket));

			_Button->setEnabled(true);

			break;
		}
		case BasicPacket::DATA_REQUESTROUTEDATA:
		{
			lprintfln("DATA_REQUESTROUTEDATA on Connect Finished(packetid : %d)", packet->PacketID);

			//_RouteIDs.clear();

			mConnection->write(packet, sizeof(RRouteData));

			//mConnection->write(packet, sizeof(NextPacketSize));

			break;
		}
		case BasicPacket::DATA_VIDEO_URLREQ:
		{
			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			packet = new VideoReqPacket();

			((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_VIDEO_URLREQ;
			((VideoReqPacket*)packet)->videoID = _PoI->getVidID();

			mConnection->write(packet, sizeof(VideoReqPacket));

			break;
		}
		case BasicPacket::DATA_IMG_URLREQ:
		{
			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			packet = new VideoReqPacket();

			((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_IMG_URLREQ;
			((VideoReqPacket*)packet)->videoID = _PoI->getImgID();

			mConnection->write(packet, sizeof(VideoReqPacket));

			break;
		}
		case BasicPacket::DATA_TEXT_REQ:
		{
			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			packet = new VideoReqPacket();

			((VideoReqPacket*)packet)->PacketID = BasicPacket::DATA_TEXT_REQ;
			((VideoReqPacket*)packet)->videoID = _PoI->getTxtID();

			mConnection->write(packet, sizeof(VideoReqPacket));

			break;
		}
	}
}

void Option::connReadFinished(Connection *conn, int result)
{

	MAExtent tSize = maGetScrSize();

	int sizeX = EXTENT_X(tSize);
	int sizeY = EXTENT_Y(tSize);


	lprintfln("Packet code: %d", packet->PacketID);
	lprintfln("Result: %d", result);

	/*if(result < 0)
	{
		maAlert("Magna Carta", "Connection: Write failed. Try again.", "Ok", NULL, NULL);

		mConnection->close();

		_Button->setEnabled(true);

		delete packet;

		return;
	}*/

	if(packet->PacketID == BasicPacket::DATA_PACKETSIZE)
		{
			BasicPacket *temp = packet;
			unsigned int packetSize;

			packetSize = sizeof(XMLPacket)+((NextPacketSize*)packet)->size;//-1;

			packet = (XMLPacket*)malloc(packetSize);

			lprintfln("Packet size is: %u", packetSize);

			delete temp;

			mConnection->read(packet, packetSize);
		}
		else if(packet->PacketID == BasicPacket::DATA_XMLDATA)
		{
			_Button->setEnabled(true);

			_RouteList->clear();

			_RouteIDs.clear();

			_RouteIDs.add(-1);
			_RouteList->addOption("Current route.");

			//parse it with the xml parser.
			_XMLParser->parse(((XMLPacket*)packet)->xmlData);

			lprintfln("Route list xml %s", ((XMLPacket*)packet)->xmlData);

			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			//_RouteList->setHeight((int)(0.9*sizeY));
			//_RouteList->setWidth(sizeX);
			//_RouteList->show(0, 0);
		}
		else if(packet->PacketID == BasicPacket::DATA_ROUTEDATA)
		{
			_XMLParser1->parse(((XMLPacket*)packet)->xmlData);

			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}
		}
		else if(packet->PacketID == BasicPacket::DATA_VIDEO_URLRES)
		{
			MAUtil::String url;

			url = BASEURL;
			url += ((VideoResPacket*)packet)->url;

			((PreviewScreen*)_MScr->getPreviewScreen())->setVideoData(url);
			_MScr->callPreviewScreen(PreviewScreen::VideoScr);
		}
		else if(packet->PacketID == BasicPacket::DATA_IMG_URLRES)
		{
			MAUtil::String url;

			url = BASEURL;
			url += ((VideoResPacket*)packet)->url;

			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			placeholder = maCreatePlaceholder();

			_Downloader.beginDownloading(url.c_str(), placeholder);
		}
		else if(packet->PacketID == BasicPacket::DATA_TEXT_RES)
		{
			MAUtil::String txt;

			txt = ((TextResPacket*)packet)->text;

			if(packet != NULL)
			{
				delete packet;

				packet = NULL;
			}

			((PreviewScreen*)_MScr->getPreviewScreen())->setTxtData(txt);
			_MScr->callPreviewScreen(PreviewScreen::TextScr);
		}
}

void Option::connWriteFinished(Connection *conn, int result)
{
	/*if(result < 0)
	{
		maAlert("Magna Carta", "Connection: Write failed. Try again.", "Ok", NULL, NULL);

		mConnection->close();

		_Button->setEnabled(true);

		delete packet;

		return;
	}*/

	lprintfln("Result: %d", result);

	//_Button->setEnabled(true);

	if(packet->PacketID == BasicPacket::DATA_REQUESTROUTES)
	{
		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		packet = new NextPacketSize();

		lprintfln("Reading next packet size");

		mConnection->read(packet, sizeof(NextPacketSize));
	}
	else if(packet->PacketID == BasicPacket::DATA_PACKETSIZE)
	{
		//Nothing should be done here because only the server send the DATA_PACKETSIZE packet.
	}
	else if(packet->PacketID == BasicPacket::DATA_REQUESTROUTEDATA)
	{
		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		packet = new NextPacketSize();

		//packet->PacketID = BasicPacket::DATA_PACKETSIZE;

		mConnection->read(packet, sizeof(NextPacketSize));
	}
	else if(packet->PacketID == BasicPacket::DATA_VIDEO_URLREQ || packet->PacketID == BasicPacket::DATA_IMG_URLREQ)
	{
		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		packet = new VideoResPacket();

		//packet->PacketID = BasicPacket::DATA_VIDEO_URLRES;

		mConnection->read(packet, sizeof(VideoResPacket));
	}
	else if(packet->PacketID == BasicPacket::DATA_TEXT_REQ)
	{
		if(packet != NULL)
		{
			delete packet;

			packet = NULL;
		}

		packet = new NextPacketSize();

		mConnection->read(packet, sizeof(NextPacketSize));
	}
}

void Option::XMLParsed(XMLBase *obj)
{
	if(obj == _XMLParser)
	{
		MAExtent tSize = maGetScrSize();

		int sizeX = EXTENT_X(tSize);
		int sizeY = EXTENT_Y(tSize);

		_RouteList->setHeight((int)(0.9*sizeY));
		_RouteList->setWidth(sizeX);
		_RouteList->show(0, 0);
	}
	else if(obj == _XMLParser1)
	{
		//MAUtil::Vector<MAP::LonLat> temp;
		MAUtil::Vector<LocData> array = _XMLParser1->getArray();

		int arraySize = array.size();

		_PinLayer->clearLayer();
		_LineLayer->clearLayer();

		for(int i = 0; i < arraySize; i++)
		{
			loc_data[i] = array[i].loc;

			if(array[i].imgID || array[i].txtID || array[i].videoID)
			{
				PoI *tmpPin = new PoI(0, 0, 12, 20, NULL);

				tmpPin->setDataIds(array[i].imgID, array[i].txtID, array[i].videoID);
				tmpPin->setLonLat(array[i].loc.lon, array[i].loc.lat);
				tmpPin->setPopUp(_PopUp);

				_PinLayer->addPin(tmpPin);
			}
		}

		_Map->getViewport()->setCenterPosition(loc_data[0], false, false);
		lprintfln("Centered at %lf, %lf", loc_data[0].lon, loc_data[0].lat);
		_LineLayer->addPoints(loc_data);
	}
}

void Option::XMLData(XMLBase *obj, MAUtil::String &Data)
{
	if(obj == _XMLParser)
	{
		lprintfln("Found string in xml: %s\n", Data.c_str());

		_RouteList->addOption(Data);
	}
	else if(obj == _XMLParser1)
	{
		lprintfln("Found string in xml: %s\n", Data.c_str());
	}
}

void Option::XMLAttr(XMLBase *obj, MAUtil::String &Data)
{
	lprintfln("Found attribute in xml: %s", Data.c_str());

	_RouteIDs.add(atoi(Data.c_str()));
}

bool Option::outOfMemory(MAUtil::Downloader *downloader)
{
	maAlert("Magna Carta", "Image downloader has run out of memory", "Ok", NULL, NULL);

	maDestroyPlaceholder(placeholder);

	placeholder = -1;
}

void Option::error(MAUtil::Downloader *downloader, int code)
{
	maAlert("Magna Carta", "An error occured in the image downloader", "Ok", NULL, NULL);

	maDestroyPlaceholder(placeholder);

	placeholder = -1;
}

PinLayer* Option::getPinLayer()
{
	return _PinLayer;
}

LineLayer* Option::getLineLayer()
{
	return _LineLayer;
}

void Option::downloadCancelled(MAUtil::Downloader *downloader)
{
	maAlert("Magna Carta", "Image download cancelled.", "Ok", NULL, NULL);

	maDestroyPlaceholder(placeholder);

	placeholder = -1;
}

void Option::finishedDownloading(MAUtil::Downloader *downloader, MAHandle data)
{
	((PreviewScreen*)_MScr->getPreviewScreen())->setImageData(data);

	_MScr->callPreviewScreen(PreviewScreen::ImageScr);
}

void Option::firstPoint(GPS *gpsWidget, MALocation loc)
{
	_Point.lat = loc.lat;
	_Point.lon = loc.lon;

	if(!tracking)
		return;

	MAP::LonLat tmp;

	tmp.lat = loc.lat;
	tmp.lon = loc.lon;

	loc_data.add(tmp);

	_LineLayer->addPoints(loc_data);

	_Map->getViewport()->setCenterPosition(tmp, false, false);
}

void Option::hasMoved(GPS* gpsWidget, MALocation loc, MALocation old_loc)
{
	_Point.lat = loc.lat;
	_Point.lon = loc.lon;

	//lprintfln("tracking value: %d", tracking);

	if(!tracking)
	{
		MALocation tmp;
		MAP::LonLat tmp2;

		tmp.lat = loc_data[index].lat;
		tmp.lon = loc_data[index].lon;

		tmp2.lat = loc.lat;
		tmp2.lon = loc.lon;

		if(gpsWidget->coordsToMeters(loc, tmp) >= 50)
		{
			index++;
		}

		_Map->getViewport()->setCenterPosition(tmp2, false, false);

		return;
	}

	if(gpsWidget->coordsToMeters(loc, old_loc) >= 5)
	{
		//lprintfln("coordsToMeters(BrowseScreen): %d", gpsWidget->coordsToMeters(loc, old_loc));
		if(tracking)
		{
			MAP::LonLat tmp;

			tmp.lat = loc.lat;
			tmp.lon = loc.lon;

			loc_data.add(tmp);

			_LineLayer->addPoints(loc_data);

			_Map->getViewport()->setCenterPosition(tmp, false, false);
		}
	}
}

MAUtil::Vector<MAP::LonLat>* Option::getLonLatArray()
{
	return &loc_data;
}

Option::~Option()
{
	_PopUp->removePopUpListener(this);
	_Button->removeButtonListener(this);
	_RouteList->removePopUpListener(this);

	if(placeholder != -1)
		maDestroyPlaceholder(placeholder);

	delete _XMLParser;
	delete _XMLParser1;
	delete _Font;
	delete _PopUp;
	delete mConnection;

	if(packet != NULL)
	{
		delete packet;

		packet = NULL;
	}
}
