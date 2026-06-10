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
		auto& zMotion = Modules::getInstance().zMotionModule.zMotion;

		if (!zMotion)
		{
			return;
		}

		if (!zMotion->isConnected())
		{
			emit zMotionDisconnect();
		}
	}
}
