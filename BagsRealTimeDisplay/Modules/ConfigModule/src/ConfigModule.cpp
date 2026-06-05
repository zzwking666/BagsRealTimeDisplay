#include"ConfigModule.hpp"

#include <oso/oso_StorageContext.hpp>

#include "utility.hpp"

bool ConfigModule::build()
{
	storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);

	loadConfigSafe(globalPath.bagsRealTimeDisplayConfigPath, bagsRealTimeDisplayInfo, QStringLiteral("主窗体参数"));
	loadConfigSafe(globalPath.setConfigPath, setConfig, QStringLiteral("设置参数"));

	return true;
}

void ConfigModule::destroy()
{
	storeContext->saveSafe(bagsRealTimeDisplayInfo, globalPath.bagsRealTimeDisplayConfigPath.toStdString());
	storeContext->saveSafe(setConfig, globalPath.setConfigPath.toStdString());
	storeContext.reset();
}

void ConfigModule::start()
{

}

void ConfigModule::stop()
{

}
