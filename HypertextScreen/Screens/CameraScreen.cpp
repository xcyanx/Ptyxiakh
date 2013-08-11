/*
 * CameraScreen.cpp
 *
 *  Created on: 3 Σεπ 2012
 *      Author: Σταμάτης
 */




#include "CameraScreen.h"
#include <mavsprintf.h>


void CameraScreen::LaunchCamera()
{
	maCaptureAction(MA_CAPTURE_ACTION_RECORD_VIDEO);
}

void CameraScreen::LaunchPhoto()
{
	maCaptureAction(MA_CAPTURE_ACTION_TAKE_PICTURE);
}

void CameraScreen::Photo(MACaptureEventData &data)
{
	String fileName = GetLocalPath()+RandomName();

	lprintfln("CameraScreen(filename): %s", fileName.c_str());

	if(GetOS() == "Android")
	{
		fileName += ".jpg";
	}
	else
	{
		fileName += ".png";
	}

	maCaptureWriteImage(data.handle, fileName.c_str(), fileName.size());

	_pathname = fileName;
	type = _PHOTO_;
}


int CameraScreen::GetPathname(String &Pathname)
{
	Pathname = _pathname;

	return type;
}


String CameraScreen::RandomName()
{
	DateTime time = DateTime::now();

	String result;

	result = MAUtil::integerToString(time.getDay());
	result += "-";
	result += MAUtil::integerToString(time.getMonth()+1);
	result += "-";
	result += MAUtil::integerToString(time.getYear()-100);
	result += " ";
	result += MAUtil::integerToString(time.getHour());
	//result += "";
	result += MAUtil::integerToString(time.getMinute());
	//result += "";
	result += MAUtil::integerToString(time.getSecond());

	return result;
}


void CameraScreen::Camera(MACaptureEventData &data)
{
	String vidPath;

	vidPath.resize(512);

	maCaptureGetVideoPath(data.handle, vidPath.pointer(), vidPath.size());

	_pathname = vidPath;
	type = _CAMERA_;
}

String CameraScreen::GetOS()
{
	//char os[64];

	String os;

	os.resize(64);

	maGetSystemProperty("mosync.device.OS", const_cast<char*>(os.c_str()), os.size());


	return os;
}

int CameraScreen::GetSystemProperty(const char* key, MAUtil::String& dst)
{
	    int size = maGetSystemProperty(key, NULL, 0);
	    if(size < 0)
	        return size;
	    dst.resize(size-1);
	    maGetSystemProperty(key, dst.pointer(), size);
	    return size;
}

String CameraScreen::GetLocalPath()
{
    // Do this here to work around a MoRE bug.
    FileLister fl;
    fl.start("/");

    MAUtil::String path, os;
    // Try getting the local path.
    int result = GetSystemProperty("mosync.path.local", path);

    GetSystemProperty("mosync.device.OS", os);


    if(MAUtil::lowerString(os).find("android", 0) != -1)
    {
    	MAHandle tfile;

    	path = "/sdcard/Magna Carta/";

    	tfile = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);

    	lprintfln("CameraScreen-File handle: %d", tfile);
    	lprintfln("CameraScreen-File exists: %d", maFileExists(tfile));


    	if(!maFileExists(tfile))
    	{
    		int result = maFileCreate(tfile);

    		lprintfln("CameraScreen-File create code: %d", result);
    	}

    	maFileClose(tfile);

    	lprintfln("CameraScreen-Path: %s", path.c_str());

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


CameraScreen::CameraScreen(): type(-1)
{
	Environment::getEnvironment().addCustomEventListener(this);
	maCaptureSetProperty(MA_CAPTURE_VIDEO_QUALITY, "1");
}

CameraScreen::~CameraScreen()
{
	Environment::getEnvironment().removeCustomEventListener(this);

	_Listeners.clear();
}

void CameraScreen::customEvent(const MAEvent &event)
{
	if(event.type == EVENT_TYPE_CAPTURE)
	{
		MACaptureEventData eventData = event.captureData;
		if(eventData.type == MA_CAPTURE_EVENT_TYPE_IMAGE)
		{
			Photo(eventData);
			type = _PHOTO_;
		}
		else if(eventData.type == MA_CAPTURE_EVENT_TYPE_VIDEO)
		{
			Camera(eventData);
			type = _CAMERA_;
		}
		else if(eventData.type == MA_CAPTURE_EVENT_TYPE_CANCEL)
		{
			_pathname = "";
			type = -1;
		}


		for(int i = 0; i < _Listeners.size(); i++)
		{
			_Listeners[i]->Notify(type);
		}


		maCaptureDestroyData(eventData.handle);
	}
}

void CameraScreen::AddCamListener(CameraListener *listener)
{
	NativeUI::addListenerToVector(_Listeners, listener);
}

void CameraScreen::RemoveCamListener(CameraListener *listener)
{
	NativeUI::removeListenerFromVector(_Listeners, listener);
}
