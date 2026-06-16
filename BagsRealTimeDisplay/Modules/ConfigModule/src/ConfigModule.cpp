#include"ConfigModule.hpp"

#include <rwul/oso/oso_StorageContext.hpp>

#include "utility.hpp"

bool ConfigModule::build()
{
	storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);

	loadConfigSafe(globalPath.bagsRealTimeDisplayConfigPath, bagsRealTimeDisplayInfo, "BagsRealTimeDisplayInfo");
	loadConfigSafe(globalPath.setConfigPath, setConfig, "SetConfig");

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
