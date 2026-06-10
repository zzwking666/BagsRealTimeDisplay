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
	_ipAddress = utility.zMotionIp.toStdString();

	zMotion = std::make_unique<rw::hoep::ZMotionDevice>();

	rw::hoep::ZMotionEthernetCfg cfg;
	cfg.ipAddress = _ipAddress;
	cfg.timeoutMs = 100;

	if (!zMotion->connect(cfg))
	{
		qWarning() << "ZMotion 连接失败:" << _ipAddress.c_str();
	}
	else
	{
		qDebug() << "ZMotion 连接成功:" << _ipAddress.c_str();
	}

	return true;
}

void ZMotionModule::destroy()
{
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
	destroy();
	return build();
}
