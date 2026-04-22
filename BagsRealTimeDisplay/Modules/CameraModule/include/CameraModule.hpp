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
	bool build_camera2();
	void destroy_camera1();
	void destroy_camera2();
public:
	void setCamera1TriggerOff();
	void setCamera1HardwareTrigger();
	void setCamera2TriggerOff();
	void setCamera2HardwareTrigger();
public:
	void setCamera1ExposureTime(size_t exposureTime);
	void seCamera1tGain(size_t gain);
	void setCamera2ExposureTime(size_t exposureTime);
	void seCamera2tGain(size_t gain);
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
private:
	std::unique_ptr<rw::hoec::DVPCameraPassive> camera1{ nullptr };
	std::unique_ptr<rw::hoec::DVPCameraPassive> camera2{ nullptr };
public:
	std::atomic<bool> isCamera1SoftTrigger{ false };
	std::atomic<bool> isCamera2SoftTrigger{ false };
};