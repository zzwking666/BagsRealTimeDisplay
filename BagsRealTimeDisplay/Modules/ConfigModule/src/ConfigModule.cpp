#include"ConfigModule.hpp"
#include "utility.hpp"

bool ConfigManagerModule::build()
{
	storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);

	auto loadBagsRealTimeDisplayInfo = storeContext->loadSafe(globalPath.bagsRealTimeDisplayConfigPath.toStdString());

	if (loadBagsRealTimeDisplayInfo)
	{
		bagsRealTimeDisplayInfo = *loadBagsRealTimeDisplayInfo;
	}
	else
	{
		qWarning("主窗体参数加载失败!");
		return false;
	}

	return true;
}

void ConfigManagerModule::destroy()
{
	storeContext->saveSafe(bagsRealTimeDisplayInfo, globalPath.bagsRealTimeDisplayConfigPath.toStdString());
	storeContext.reset();
}

void ConfigManagerModule::start()
{

}

void ConfigManagerModule::stop()
{

}
