#pragma once

#include"IModule.hpp"
#include<QObject>
#include <QDebug>
#include <QFile>
#include "rwul/oso/oso_StorageContext.hpp"
#include "rwul/oso/oso_func.hpp"
#include "BagsRealTimeDisplayInfo.hpp"
#include "SetConfig.hpp"

class ConfigModule
	: public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
public:
	std::unique_ptr<rw::oso::StorageContext> storeContext{ nullptr };
public:
	cdm::BagsRealTimeDisplayInfo bagsRealTimeDisplayInfo{};
	cdm::SetConfig setConfig{};

public:
	template<class ConfigType>
	bool loadConfigSafe(const QString& path, ConfigType& outConfig, const QString& configName);
};

template <class ConfigType>
bool ConfigModule::loadConfigSafe(const QString& path, ConfigType& outConfig, const QString& configName)
{
	if (!QFile::exists(path))
	{
		qWarning() << configName << "配置文件不存在:" << path;
		return false;
	}

	// 优先使用 loadSafeToType 尝试加载
	bool isLoad = false;
	outConfig = storeContext->loadSafeToType<ConfigType>(path.toStdString(), isLoad);

	if (isLoad)
	{
		return true;
	}

	// loadSafeToType 失败，尝试通过合并默认值来修复
	auto loadData = storeContext->loadSafe(path.toStdString());
	if (!loadData)
	{
		qWarning() << configName << "配置文件加载失败，删除:" << path;
		QFile::remove(path);
		return false;
	}

	qWarning() << configName << "配置文件字段不匹配，尝试合并默认值...";

	rw::oso::ObjectStoreAssembly oldAssembly = *loadData;
	rw::oso::ObjectStoreAssembly newAssembly = ConfigType();
	rw::oso::AssemblyMergeTool::Merge(newAssembly, oldAssembly);

	if (storeContext->saveSafe(newAssembly, path.toStdString()))
	{
		qDebug() << configName << "配置文件更新成功:" << path;

		// 合并后重新尝试构造
		try
		{
			outConfig = ConfigType(newAssembly);
			return true;
		}
		catch (...)
		{
			qWarning() << configName << "合并后仍无法解析，使用默认值";
		}
	}
	else
	{
		qWarning() << configName << "合并后保存失败:" << path;
	}
	return false;
}
