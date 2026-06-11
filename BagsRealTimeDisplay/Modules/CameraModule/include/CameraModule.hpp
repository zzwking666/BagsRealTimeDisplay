#pragma once
#include <memory>
#include <QObject>

#include"IModule.hpp"
#include"rwul/rqwcd/rqwc_d.hpp"

#ifdef BUILD_WITHOUT_HARDWARE
#include <QTimer>
#endif

class CameraModule
	: public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	CameraModule();
	~CameraModule() override;
public:
	bool build() override;
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
	void setCamera1Gain(size_t gain);
	void setCamera2ExposureTime(size_t exposureTime);
	void setCamera2Gain(size_t gain);
public:
	void setCamera1Multiplier(size_t multiplier);
	void setCamera1PostDivider(size_t postDivider);
	void setCamera2Multiplier(size_t multiplier);
	void setCamera2PostDivider(size_t postDivider);
public:
	void setCamera1LineHeight(unsigned int lineHeight);
	void setCamera2LineHeight(unsigned int lineHeight);
private:
	bool isTargetCamera(const QString& cameraIndex, const QString& targetName);
	rw::hoec::CameraInfo cameraMetaDataCheck(const QString& cameraIndex,
		const std::vector<rw::hoec::CameraInfo>& cameraInfo);
public slots:
	bool onBuildCamera(int index);
	void onDestroyCamera(int index);
	void onStartCamera(int index);
private slots:
	void onCamera1Capture(const rw::hoec::MatInfo& matInfo);
	void onCamera2Capture(const rw::hoec::MatInfo& matInfo);
#ifdef BUILD_WITHOUT_HARDWARE
	void onNoHardwareCapture1();
	void onNoHardwareCapture2();
#endif
signals:
	void onCameraCapture(const rw::hoec::MatInfo& matInfo, size_t index);

	void onCameraStateChanged(int index, bool state);
private:
	std::unique_ptr<rw::rqwc::DVPCameraPassive> camera1{ nullptr };
	std::unique_ptr<rw::rqwc::DVPCameraPassive> camera2{ nullptr };
#ifdef BUILD_WITHOUT_HARDWARE
	QTimer* noHardwareTimer1{ nullptr };
	QTimer* noHardwareTimer2{ nullptr };
	int noHardwareFrameNum1{ 0 };
	int noHardwareFrameNum2{ 0 };
#endif
public:
	std::atomic<bool> isCamera1SoftTrigger{ false };
	std::atomic<bool> isCamera2SoftTrigger{ false };
};