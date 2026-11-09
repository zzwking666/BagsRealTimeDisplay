#include "AppRuntime.hpp"

bool AppRuntime::initialize()
{
    if (!Modules::check())
    {
        return false;
    }

	_modules.build();

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
	// 先切断异步数据源到界面的连接，避免退出过程中继续刷新正在销毁的控件。
	if (_bagsRealTimeDisplay)
	{
		QObject::disconnect(&_modules.imageStitchModule, nullptr,
			_bagsRealTimeDisplay.get(), nullptr);
		QObject::disconnect(&_modules.cameraModule, nullptr,
			_bagsRealTimeDisplay.get(), nullptr);
	}
	if (_modules.asynchronousThreadModule.refreshUIThread)
	{
		QObject::disconnect(_modules.asynchronousThreadModule.refreshUIThread.get(),
			nullptr, nullptr, nullptr);
	}

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
}
