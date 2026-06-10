#include "AsynchronousThreadModule.hpp"

bool AsynchronousThreadModule::build()
{
	refreshUIThread = std::make_unique<RefreshUIThread>(this);
	zMotionPollingThread = std::make_unique<ZMotionPollingThread>(this);

	return true;
}

void AsynchronousThreadModule::destroy()
{
	if (zMotionPollingThread)
	{
		zMotionPollingThread.reset();
	}

	if (refreshUIThread)
	{
		refreshUIThread.reset();
	}
}

void AsynchronousThreadModule::start()
{
	if (refreshUIThread)
	{
		refreshUIThread->startThread();
	}

	if (zMotionPollingThread)
	{
		zMotionPollingThread->startThread();
	}
}

void AsynchronousThreadModule::stop()
{
	if (zMotionPollingThread)
	{
		zMotionPollingThread->stopThread();
	}

	if (refreshUIThread)
	{
		refreshUIThread->stopThread();
	}
}
