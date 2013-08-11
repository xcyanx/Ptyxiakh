/*
 * CameraScreen.h
 *
 *  Created on: 5 Αυγ 2012
 *      Author: Σταμάτης
 */

#ifndef CAMERASCREEN_H_
#define CAMERASCREEN_H_


#include <MAUtil/Environment.h>
#include <MAUtil/String.h>
#include <MAP/DateTime.h>
#include <NativeUI/Widgets.h>
#include <maapi.h>
#include <MAUtil/FileLister.h>

#include "CameraListener.h"

using namespace MAUtil;
using namespace MAPUtil;

class CameraScreen: public CustomEventListener
{
	public:
		enum {_CAMERA_, _PHOTO_};

	public:
		CameraScreen();
		~CameraScreen();
	    int GetPathname(String &Pathname);
	    void LaunchCamera();
	    void LaunchPhoto();
	    void AddCamListener(CameraListener *listener);
	    void RemoveCamListener(CameraListener *listener);

	private:
		void Photo(MACaptureEventData &data);
		void Camera(MACaptureEventData &data);
		int GetSystemProperty(const char* key, MAUtil::String& dst);
		String RandomName();
		String GetOS();
		String GetLocalPath();
		virtual void customEvent(const MAEvent &event);

	private:
		String _pathname;
		int type;
		Vector<CameraListener*> _Listeners;
};


#endif /* CAMERASCREEN_H_ */
