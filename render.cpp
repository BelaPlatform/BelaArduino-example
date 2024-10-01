#include <libraries/BelaLibpd/BelaLibpd.h>
#include <libraries/BelaArduino/BelaArduino.h>
#include <libraries/Watcher/Watcher.h>

Gui wmGui;
WatcherManager wm(wmGui);
Watcher<float> myvar("myvar"); // this is the user-define watcher

bool setup(BelaContext* context, void* userData)
{
	BelaLibpdSettings settings;
	settings.useIoThreaded = false;
	settings.useGui = false; // we want to use the BelaArduino GUI, which is controlled here and in ino.cpp
	settings.messageHook = BelaArduino_messageHook;
	settings.listHook = BelaArduino_listHook;
	settings.floatHook = BelaArduino_floatHook;
	if(!BelaLibpd_setup(context, userData, settings))
		return false;
	BelaArduinoSettings arduinoSettings {};
	if(!BelaArduino_setup(context, userData, arduinoSettings))
		return false;
	unsigned int wsPort = 5557;
	wm.getGui().setup("/libraries/Watcher/sketch-watcher-example.js", wsPort);
	printf("sketch-watcher.js GUI at bela.local/gui?wsPort=%u\n", wsPort);
	return true;
}

void render(BelaContext* context, void* userData)
{
	if(wm.getGui().numConnections())
		wm.tick(context->audioFramesElapsed);
	myvar = myvar + 0.001;
	if(myvar >= 1)
		myvar = 0;
	BelaArduino_renderTop(context, userData);
	BelaLibpd_render(context, userData);
	BelaArduino_renderBottom(context, userData);
}

void cleanup(BelaContext* context, void* userData)
{
	BelaArduino_cleanup(context, userData);
	BelaLibpd_cleanup(context, userData);
}
