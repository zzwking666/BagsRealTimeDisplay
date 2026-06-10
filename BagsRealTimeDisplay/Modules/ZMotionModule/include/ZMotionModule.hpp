#pragma once
#include <QObject>
#include <memory>
#include <string>
#include "IModule.hpp"
#include "ZMotionPollingThread.hpp"

namespace rw { namespace hoep { class ZMotionDevice; } }

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
	// 轮询线程
	std::unique_ptr<ZMotionPollingThread> pollingThread{nullptr};

	// Zmotion 设备实例（销毁顺序在 pollingThread 之后，由析构函数保证）
	std::unique_ptr<rw::hoep::ZMotionDevice> zMotion{nullptr};

public:
	// 参数写入接口
	void setGearRatio(double ratio);
	void setPulseEquivalent(double equivalent);

private:
	std::string _ipAddress;
};
