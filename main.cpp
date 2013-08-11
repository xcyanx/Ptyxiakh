#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

#include <conprint.h>
#include <mavsprintf.h>
#include "MainScreen.h"

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet
{

private:
	MainScreen *mainScreen;

public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet()
	{
		mainScreen = new MainScreen();
		mainScreen->show();
	}

	/**
	 * Destructor.
	 */
	~NativeUIMoblet()
	{
		//close();
	}


	void NativeUIMoblet::closeEvent() GCCATTRIB(noreturn)
	{
		delete mainScreen;
		close();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			if(mainScreen->getStackSize() > 1)
			{
				mainScreen->pop();
			}
			else
			{
				//getScreen() returns the Screen of the MainScreen class
				//return { this->screen; } where screen is type of Screen.
				bool visible = mainScreen->getScreen()->isVisible();
				bool visible2 = mainScreen->getOptScr()->isVisible();

				mainScreen->pop();

				if(visible && !visible2)
				{
					closeEvent();
				}
			}
		}
	}


};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
