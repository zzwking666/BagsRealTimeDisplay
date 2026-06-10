#include "ZMotionPollingThread.hpp"
#include "rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp"

ZMotionPollingThread::ZMotionPollingThread(rw::hoep::ZMotionDevice* device,
                                           ZMotionStatus* status,
                                           QObject* parent)
	: _device(device)
	, _status(status)
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
		if (_device && _device->isConnected())
		{
			float position = 0;
			auto ret = _device->getAxisPosition(0, position);
			if (ret)
			{
				_status->currentPosition.store(static_cast<double>(position));
				_status->isConnected.store(true);
			}
			else
			{
				_status->isConnected.store(false);
			}

			float speed = 0;
			_device->getAxisSpeed(0, speed);
			_status->axisStatus.store(static_cast<int32_t>(speed));
		}
		else
		{
			_status->isConnected.store(false);
		}
		emit zMotionStatusUpdated();
		QThread::msleep(100);
	}
}
