#include"CameraModule.hpp"
#include"Utility.hpp"
#include <QRegularExpression>

#include "Modules.hpp"

CameraModule::CameraModule()
{

}

CameraModule::~CameraModule()
{

}

bool CameraModule::build()
{
	if (!build_camera1())
	{
		qWarning("Camera 1 build failed");
	}
	if (!build_camera2())
	{
		qWarning("Camera 2 build failed");
	}
	return true;
}

void CameraModule::destroy()
{
	destroy_camera1();
	destroy_camera2();
}

void CameraModule::start()
{
#ifdef BUILD_WITHOUT_HARDWARE
	if (noHardwareTimer1)
	{
		noHardwareTimer1->start();
		onCameraStateChanged(1, true);
	}
	else
	{
		onCameraStateChanged(1, false);
	}

	if (noHardwareTimer2)
	{
		noHardwareTimer2->start();
		onCameraStateChanged(2, true);
	}
	else
	{
		onCameraStateChanged(2, false);
	}
#else
	if (camera1)
	{
		camera1->startMonitor();
		onCameraStateChanged(1, true);
	}
	else
	{
		onCameraStateChanged(1, false);
	}

	if (camera2)
	{
		camera2->startMonitor();
		onCameraStateChanged(2, true);
	}
	else
	{
		onCameraStateChanged(2, false);
	}
#endif
}

void CameraModule::stop()
{
#ifdef BUILD_WITHOUT_HARDWARE
	if (noHardwareTimer1)
	{
		noHardwareTimer1->stop();
	}
	if (noHardwareTimer2)
	{
		noHardwareTimer2->stop();
	}
#else
	if (camera1)
	{
		camera1->stopMonitor();
		camera1->disconnectCamera();
	}
	if (camera2)
	{
		camera2->stopMonitor();
		camera2->disconnectCamera();
	}
#endif
}

bool CameraModule::build_camera1()
{
#ifdef BUILD_WITHOUT_HARDWARE
	noHardwareTimer1 = new QTimer(this);
	noHardwareTimer1->setInterval(500);
	QObject::connect(noHardwareTimer1, &QTimer::timeout,
		this, &CameraModule::onNoHardwareCapture1);
	onCameraStateChanged(1, true);
	return true;
#else
	auto cameraList = rw::hoec::UtilityFunc<rw::hoec::CameraProvider::DVP>::getCameraInfoList();
	auto cameraMetaData1 = cameraMetaDataCheck(utility.cameraIp1, cameraList);

	if (cameraMetaData1.ip != "0")
	{
		try
		{
			camera1 = std::make_unique<rw::rqwc::DVPCameraPassive>();
			camera1->setIP(cameraMetaData1.ip);
			camera1->connectCamera();
			camera1->registerCallBackFunc();
			//camera1->startMonitor();
			setCamera1HardwareTrigger();

			auto cfg = Modules::getInstance().configModule.setConfig;

			camera1->setExposureTime(static_cast<size_t>(cfg.baoguang1));
			camera1->setGain(static_cast<size_t>(cfg.zengyi1));

			QObject::connect(camera1.get(), &rw::rqwc::DVPCameraPassive::callBackFuncPost,
				this, &CameraModule::onCamera1Capture);

			onCameraStateChanged(1, true);
			return true;
		}
		catch (const std::exception&)
		{
			onCameraStateChanged(1, false);
			return false;
		}
	}
	onCameraStateChanged(1, false);
	return false;
#endif
}

bool CameraModule::build_camera2()
{
#ifdef BUILD_WITHOUT_HARDWARE
	noHardwareTimer2 = new QTimer(this);
	noHardwareTimer2->setInterval(500);
	QObject::connect(noHardwareTimer2, &QTimer::timeout,
		this, &CameraModule::onNoHardwareCapture2);
	onCameraStateChanged(2, true);
	return true;
#else
	auto cameraList = rw::hoec::UtilityFunc<rw::hoec::CameraProvider::DVP>::getCameraInfoList();
	auto cameraMetaData2 = cameraMetaDataCheck(utility.cameraIp2, cameraList);

	if (cameraMetaData2.ip != "0")
	{
		try
		{
			camera2 = std::make_unique<rw::rqwc::DVPCameraPassive>();
			camera2->setIP(cameraMetaData2.ip);
			camera2->connectCamera();
			camera2->registerCallBackFunc();
			//camera2->startMonitor();
			setCamera2HardwareTrigger();

			auto cfg = Modules::getInstance().configModule.setConfig;

			camera2->setExposureTime(static_cast<size_t>(cfg.baoguang2));
			camera2->setGain(static_cast<size_t>(cfg.zengyi2));

			QObject::connect(camera2.get(), &rw::rqwc::DVPCameraPassive::callBackFuncPost,
				this, &CameraModule::onCamera2Capture);

			onCameraStateChanged(2, true);
			return true;
		}
		catch (const std::exception&)
		{
			onCameraStateChanged(2, false);
			return false;
		}
	}
	onCameraStateChanged(2, false);
	return false;
#endif
}

void CameraModule::destroy_camera1()
{
#ifdef BUILD_WITHOUT_HARDWARE
	if (noHardwareTimer1)
	{
		noHardwareTimer1->stop();
		delete noHardwareTimer1;
		noHardwareTimer1 = nullptr;
	}
	onCameraStateChanged(1, false);
#else
	if (camera1)
	{
		camera1.reset();
		onCameraStateChanged(1, false);
	}
#endif
}

void CameraModule::destroy_camera2()
{
#ifdef BUILD_WITHOUT_HARDWARE
	if (noHardwareTimer2)
	{
		noHardwareTimer2->stop();
		delete noHardwareTimer2;
		noHardwareTimer2 = nullptr;
	}
	onCameraStateChanged(2, false);
#else
	if (camera2)
	{
		camera2.reset();
		onCameraStateChanged(2, false);
	}
#endif
}

void CameraModule::setCamera1TriggerOff()
{
	if (camera1)
	{
		camera1->setTriggerModeStatus(rw::hoec::TriggerModeStatus::OFF);
		isCamera1SoftTrigger = true;
	}
}

void CameraModule::setCamera1HardwareTrigger()
{
	if (camera1)
	{
		camera1->setTriggerModeStatus(rw::hoec::TriggerModeStatus::ON);
		//camera1->setTriggerSource(rw::hoec::TriggerSource::Line0);
		isCamera1SoftTrigger = false;
	}
}

void CameraModule::setCamera2TriggerOff()
{
	if (camera2)
	{
		camera2->setTriggerModeStatus(rw::hoec::TriggerModeStatus::OFF);
		isCamera2SoftTrigger = true;
	}
}

void CameraModule::setCamera2HardwareTrigger()
{
	if (camera2)
	{
		camera2->setTriggerModeStatus(rw::hoec::TriggerModeStatus::ON);
		//camera2->setTriggerSource(rw::hoec::TriggerSource::Line0);
		isCamera2SoftTrigger = false;
	}
}

void CameraModule::setCamera1ExposureTime(size_t exposureTime)
{
	if (camera1)
	{
		camera1->setExposureTime(exposureTime);
	}
}

void CameraModule::setCamera1Gain(size_t gain)
{
	if (camera1)
	{
		camera1->setGain(gain);
	}
}

void CameraModule::setCamera2ExposureTime(size_t exposureTime)
{
	if (camera2)
	{
		camera2->setExposureTime(exposureTime);
	}
}

void CameraModule::setCamera2Gain(size_t gain)
{
	if (camera2)
	{
		camera2->setGain(gain);
	}
}

void CameraModule::setCamera1Multiplier(size_t multiplier)
{
	if (camera1)
	{
		camera1->setMultiplier(multiplier);
	}
}

void CameraModule::setCamera1PostDivider(size_t postDivider)
{
	if (camera1)
	{
		camera1->setPostDivider(postDivider);
	}
}

void CameraModule::setCamera2Multiplier(size_t multiplier)
{
	if (camera2)
	{
		camera2->setMultiplier(multiplier);
	}
}

void CameraModule::setCamera2PostDivider(size_t postDivider)
{
	if (camera2)
	{
		camera2->setPostDivider(postDivider);
	}
}

void CameraModule::setCamera1LineHeight(unsigned int lineHeight)
{
	if (camera1)
	{
		camera1->setLineHeight(lineHeight);
	}
}

void CameraModule::setCamera2LineHeight(unsigned int lineHeight)
{
	if (camera2)
	{
		camera2->setLineHeight(lineHeight);
	}
}

bool CameraModule::isTargetCamera(const QString& cameraIndex, const QString& targetName)
{
	QRegularExpression regex(R"((\d+)\.(\d+)\.(\d+)\.(\d+))");
	QRegularExpressionMatch match = regex.match(targetName);

	if (match.hasMatch()) {
		auto matchString = match.captured(3);

		return cameraIndex == matchString;
	}

	return false;
}

rw::hoec::CameraInfo CameraModule::cameraMetaDataCheck(const QString& cameraIndex,
	const std::vector<rw::hoec::CameraInfo>& cameraInfo)
{
	for (const auto& cameraMetaData : cameraInfo) {
		if (isTargetCamera(cameraIndex, QString::fromStdString(cameraMetaData.ip))) {
			return cameraMetaData;
		}
	}
	rw::hoec::CameraInfo error;
	error.ip = "0";
	return error;
}

bool CameraModule::onBuildCamera(int index)
{
	switch (index)
	{
	case 1:
		return build_camera1();
	case 2:
		return build_camera2();
	default:
		return false;
	}
}

void CameraModule::onDestroyCamera(int index)
{
	switch (index)
	{
	case 1:
		destroy_camera1();
		break;
	case 2:
		destroy_camera2();
		break;
	default:
		break;
	}
}

void CameraModule::onStartCamera(int index)
{
	switch (index)
	{
	case 1:
		if (camera1)
		{
			camera1->startMonitor();
		}
		break;
	case 2:
		if (camera2)
		{
			camera2->startMonitor();
		}
		break;
	default:
		break;
	}
}

void CameraModule::onCamera1Capture(const rw::hoec::MatInfo& matInfo)
{
	emit onCameraCapture(matInfo, 1);
}

void CameraModule::onCamera2Capture(const rw::hoec::MatInfo& matInfo)
{
	emit onCameraCapture(matInfo, 2);
}

#ifdef BUILD_WITHOUT_HARDWARE
void CameraModule::onNoHardwareCapture1()
{
	const QString imagePath = globalPath.testImgRootPath + "OK20251225160441565_heat.jpg";
	cv::Mat img = cv::imread(imagePath.toStdString());
	if (img.empty())
	{
		qWarning("NoHardware: camera1Img not found or empty");
		return;
	}

	rw::hoec::MatInfo matInfo;
	matInfo.mat = img;
	matInfo.frameInfo.frameNum = ++noHardwareFrameNum1;
	matInfo.frameInfo.width = img.cols;
	matInfo.frameInfo.height = img.rows;

	emit onCameraCapture(matInfo, 1);
}

void CameraModule::onNoHardwareCapture2()
{
	const QString imagePath = globalPath.testImgRootPath + "OK20260602142550102.jpg";
	cv::Mat img = cv::imread(imagePath.toStdString());
	if (img.empty())
	{
		qWarning("NoHardware: camera2Img not found or empty");
		return;
	}

	rw::hoec::MatInfo matInfo;
	matInfo.mat = img;
	matInfo.frameInfo.frameNum = ++noHardwareFrameNum2;
	matInfo.frameInfo.width = img.cols;
	matInfo.frameInfo.height = img.rows;

	emit onCameraCapture(matInfo, 2);
}
#endif
