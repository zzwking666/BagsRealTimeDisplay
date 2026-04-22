#pragma once
#include <memory>
#include <QObject>

#include"IModule.hpp"
#include"hoecd/hoec_d.hpp"

enum BuildError
{
	Camera1Error = 1,
	Camera2Error = 2
};

class CameraModule
	: public QObject, public IModule<std::vector<BuildError>>
{
	Q_OBJECT
public:
	CameraModule();
	~CameraModule() override;
public:
	std::vector<BuildError> build() override;
	void destroy() override;
public:
	void start() override;
	void stop() override;
private:
	bool build_camera1();

	void destroy_camera1();

public:
	void setCameraTriggerOff();
	void setHardwareTrigger();
public:
	void setCameraExposureTime(size_t exposureTime);
	void setGain(size_t gain);
private:
	std::vector<BuildError> _buildResults;
public:
	std::vector<BuildError> getBuildResults() { return _buildResults; };

private:
	bool isTargetCamera(const QString& cameraIndex, const QString& targetName);
	rw::hoec::CameraInfo cameraMetaDataCheck(const QString& cameraIndex,
		const std::vector<rw::hoec::CameraInfo>& cameraInfo);
public slots:
	bool onBuildCamera(int index);
	void onDestroyCamera(int index);
	void onStartCamera(int index);
public:
	std::unique_ptr<rw::hoec::DVPCameraPassive> camera1{ nullptr };
	std::unique_ptr<rw::hoec::DVPCameraPassive> camera2{ nullptr };

	std::atomic<bool> isCamera1SoftTrigger{ false };
};