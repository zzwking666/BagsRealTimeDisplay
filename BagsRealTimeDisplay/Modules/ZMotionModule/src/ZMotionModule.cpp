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

	// 创建轮询线程（无论连接成功与否都创建，线程内部会处理设备未连接的情况）
	pollingThread = std::make_unique<ZMotionPollingThread>();

	return true;
}

void ZMotionModule::destroy()
{
	if (pollingThread)
	{
		pollingThread.reset();
	}

	if (zMotion)
	{
		zMotion->disconnect();
		zMotion.reset();
	}
}

void ZMotionModule::start()
{
	if (pollingThread)
	{
		pollingThread->startThread();
	}
}

void ZMotionModule::stop()
{
	if (pollingThread)
	{
		pollingThread->stopThread();
	}
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
