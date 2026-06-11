#pragma once
#include <QObject>
#include <memory>
#include <string>
#include "IModule.hpp"
#include "rwul/hoepZMotion/hoepZMotion_ZMotionDeviceScheduler.hpp"
#include "rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp"

class ZMotionModule
	: public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	ZMotionModule();
	~ZMotionModule() override;
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;

public:
	// Zmotion 设备实例
	std::shared_ptr<rw::hoep::ZMotionDevice> zMotion{nullptr};
	std::shared_ptr<rw::hoep::ZMotionDeviceScheduler> zMotionScheduler{ nullptr };
public:
	bool reBuildzMotion();
};
