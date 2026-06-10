#pragma once
#include <QObject>
#include <memory>
#include <string>
#include "IModule.hpp"

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
	// Zmotion 设备实例
	std::unique_ptr<rw::hoep::ZMotionDevice> zMotion{nullptr};

public:
	bool reBuildzMotion();
private:
	std::string _ipAddress;
};
