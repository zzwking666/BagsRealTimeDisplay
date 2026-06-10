#include "ZMotionPollingThread.hpp"

#include <rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp>

#include "Modules.hpp"

ZMotionPollingThread::ZMotionPollingThread(QObject* parent)
{
}

ZMotionPollingThread::~ZMotionPollingThread()
{
	stopThread();
	wait();
}

void ZMotionPollingThread::startThread()
{
	_running = true;
	if (!isRunning()) {
		start();
	}
}

void ZMotionPollingThread::stopThread()
{
	_running = false;
}

void ZMotionPollingThread::run()
{
	while (_running)
	{
		QThread::sleep(5);
		auto& zMotionModule = Modules::getInstance().zMotionModule;

		if (!zMotionModule.zMotion)
		{
			return;
		}

		if (!zMotionModule.zMotion->isConnected())
		{
			emit zMotionDisconnect();
			auto result = zMotionModule.reBuildzMotion();
		}
	}
}
