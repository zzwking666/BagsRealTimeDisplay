#include "AppRuntime.hpp"

#include "LicenseManager.hpp"
#include "DlgLicense.h"
#include "LanguageManager.h"

#include <QDebug>
#include <QDialog>
#include <QMessageBox>

bool AppRuntime::initialize()
{
    if (!Modules::check())
    {
        return false;
    }

//#pragma region check license
//#ifndef BUILD_WITHOUT_HARDWARE
//	// 未授权时弹出 DlgLicense 引导激活；取消或激活失败则中止启动
//	if (!LicenseManager::verifyAtStartup())
//	{
//		DlgLicense dlg(LicenseManager::getMachineCode());
//		if (dlg.exec() != QDialog::Accepted)
//		{
//			return false;
//		}
//		if (!LicenseManager::applyActivationCode(dlg.activationCode()))
//		{
//			return false;
//		}
//		QMessageBox::information(nullptr, "提示",
//			QString("激活成功，%1").arg(LicenseManager::getAuthorizationExpiry()));
//	}
//#else
//	qDebug() << "[开发模式] 跳过授权校验";
//#endif
//#pragma endregion

	_modules.build();

	// 按持久化的语言设置安装翻译器，必须在构造主窗口之前（setupUi 时即按当前语言生成文本）
	LanguageManager::getInstance().applyLanguage(_modules.configModule.setConfig.yuyan);

	_bagsRealTimeDisplay = std::make_unique<BagsRealTimeDisplay>(_modules.configModule, _modules.cameraModule);

	_modules.connect();

    build_connect();

    _modules.start();

    return true;
}

void AppRuntime::show()
{
#ifdef NDEBUG
    _bagsRealTimeDisplay->showFullScreen();
#else
    _bagsRealTimeDisplay->show();
#endif
}

void AppRuntime::shutdown()
{
	_modules.stop();
    _modules.destroy();
}

void AppRuntime::build_connect()
{
	// 连接相机模块的图像捕获信号到UI显示槽函数
    QObject::connect(&_modules.imageStitchModule, &ImageStitch::imageReady,
        _bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::onCameraDisplay);

	// 连接相机模块的状态变化信号到UI显示槽函数
    QObject::connect(&_modules.cameraModule, &CameraModule::onCameraStateChanged,
        _bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::updateCameraLabelState);

    // 连接异步线程刷新窗体UI
    QObject::connect(_modules.asynchronousThreadModule.refreshUIThread.get(), &RefreshUIThread::emit_RefreshUI,
		_bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::onUpdateStatisticalInfoUI);
    QObject::connect(_modules.asynchronousThreadModule.refreshUIThread.get(), &RefreshUIThread::emit_RefreshUI,
        _bagsRealTimeDisplay.get()->_dlgProductSet, &DlgProductSet::onUpdateFrameLost);

    QObject::connect(_bagsRealTimeDisplay.get()->_dlgProductSet, &DlgProductSet::paramsChanged,
        _bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::setConfigWindowClosed);

    // 设置对话框切换语言 → 语言管理器安装/卸载翻译器 → 主窗口级联重刷所有界面文本
    QObject::connect(_bagsRealTimeDisplay.get()->_dlgProductSet, &DlgProductSet::emit_changeLanguage,
        &LanguageManager::getInstance(), &LanguageManager::applyLanguage);
    QObject::connect(&LanguageManager::getInstance(), &LanguageManager::languageChanged,
        _bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::retranslate);

    QObject::connect(_modules.asynchronousThreadModule.zMotionPollingThread.get(), &ZMotionPollingThread::zMotionDisconnect,
        _bagsRealTimeDisplay.get(), &BagsRealTimeDisplay::onZMotionDisconnect);
}
