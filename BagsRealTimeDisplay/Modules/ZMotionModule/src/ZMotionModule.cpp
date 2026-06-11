#include "ZMotionModule.hpp"
#include "rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp"
#include <QDebug>

#include "Utility.hpp"


ZMotionModule::ZMotionModule()
{
}

ZMotionModule::~ZMotionModule()
{
}

bool ZMotionModule::build()
{
	rw::hoep::ZMotionEthernetCfg cfg;
	cfg.ipAddress = utility.zMotionIp.toStdString();
	cfg.timeoutMs = 100;

	zMotion = std::make_shared<rw::hoep::ZMotionDevice>(cfg);

	if (!zMotion->connect())
	{
		qWarning() << "ZMotion 连接失败:" << utility.zMotionIp.toStdString().c_str();
	}
	else
	{
		qDebug() << "ZMotion 连接成功:" << utility.zMotionIp.toStdString().c_str();
	}

	zMotionScheduler = std::make_shared<rw::hoep::ZMotionDeviceScheduler>(zMotion);

	return true;
}

void ZMotionModule::destroy()
{
	if (zMotionScheduler)
	{
		zMotionScheduler.reset();
	}
	if (zMotion)
	{
		zMotion->disconnect();
		zMotion.reset();
	}
}

void ZMotionModule::start()
{
}

void ZMotionModule::stop()
{
}

bool ZMotionModule::reBuildzMotion()
{
	return zMotion->reconnect();
}
