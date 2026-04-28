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
