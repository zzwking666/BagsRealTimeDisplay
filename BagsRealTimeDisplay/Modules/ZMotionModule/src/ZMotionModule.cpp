#include "ZMotionModule.hpp"
#include "rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp"
#include <QDebug>

ZMotionModule::ZMotionModule()
{
}

ZMotionModule::~ZMotionModule()
{
}

bool ZMotionModule::build()
{
	// TODO: 从 ConfigModule 读取 IP 地址配置
	_ipAddress = "192.168.1.250";

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

void ZMotionModule::setGearRatio(double ratio)
{
	if (zMotion && zMotion->isConnected())
	{
		// 通过 ModBus 或其他方式设置电子齿轮比
		qDebug() << "ZMotion setGearRatio:" << ratio;
	}
}

void ZMotionModule::setPulseEquivalent(double equivalent)
{
	if (zMotion && zMotion->isConnected())
	{
		// 设置轴脉冲当量
		zMotion->setAxisUnits(0, static_cast<float>(equivalent));
		qDebug() << "ZMotion setPulseEquivalent:" << equivalent;
	}
}
