#include "LicenseManager.hpp"

#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iterator>
#include <sstream>
#include <string>

#include "actCry_ActivationInfo.hpp"
#include "rwul/hoei/hoei_CPUInfo.hpp"
#include "rwul/hoei/hoei_MotherBoardInfo.hpp"

namespace
{
	// 产品名与加密密钥，建议后续通过构建参数或独立密钥文件注入
	constexpr char kProductName[] = "BagsRealTimeDisplay";
	constexpr char kActivationKey[] = "BagsRealTimeDisplayActivationKey";

	/// 将字符串转为大写
	std::string toUpper(const std::string& str)
	{
		std::string result;
		result.reserve(str.size());
		for (unsigned char ch : str)
		{
			result.push_back(static_cast<char>(std::toupper(ch)));
		}
		return result;
	}

	/// 判定主板 UUID 是否有效
	bool isValidMotherboardUuid(const std::string& uuid)
	{
		if (uuid.empty())
			return false;

		const std::string upper = toUpper(uuid);
		if (upper == "UNKNOWN UUID")
			return false;
		if (upper == "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF")
			return false;
		if (upper.find("O.E.M") != std::string::npos)
			return false;
		if (upper.find("TO BE FILLED") != std::string::npos)
			return false;

		return true;
	}

	/// 使用 WMIC 读取首个物理磁盘序列号，作为主板 UUID 无效时的 fallback
	std::string getSystemDiskSerialNumber()
	{
		try
		{
			char buffer[256];
			FILE* pipe = _popen("wmic path Win32_DiskDrive where Index=0 get SerialNumber", "r");
			if (!pipe)
				return {};

			std::string result;
			while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
			{
				result += buffer;
			}
			_pclose(pipe);

			std::istringstream stream(result);
			std::string line;
			std::getline(stream, line); // 跳过表头
			std::getline(stream, line); // 获取序列号

			line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char ch) {
				return std::isspace(ch) || ch == '.';
			}), line.end());

			return line;
		}
		catch (...)
		{
			return {};
		}
	}

	/// 生成原始硬件指纹字符串
	std::string generateHardwareFingerprint()
	{
		std::string mbUuid = rw::hoei::MotherBoardInfoFactory::GetMotherboardUniqueID();
		std::string cpuModel = rw::hoei::CPUInfoFactory::GetCPUModel();
		std::string diskSerial = getSystemDiskSerialNumber();

		if (!isValidMotherboardUuid(mbUuid))
			mbUuid = "UNKNOWN_MB";

		std::string combined = std::string(kProductName) + "|" + mbUuid + "|" + cpuModel + "|" + diskSerial;
		return combined;
	}

	/// 构造注册表配置
	rw::actCry::ActivationInfoRegistryCfg getRegistryCfg()
	{
		rw::actCry::ActivationInfoRegistryCfg cfg;
		cfg.name = kProductName;
		cfg.keyPath = "Software\\RW\\ActCry\\";
		cfg.valueName = "ActivationInfo";
		cfg.generateCodeKey = kActivationKey;
		return cfg;
	}
}

QString LicenseManager::getMachineCode()
{
	std::string fingerprint = generateHardwareFingerprint();
	QByteArray data(fingerprint.data(), static_cast<int>(fingerprint.size()));
	return QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
}

bool LicenseManager::isAuthorized()
{
	const QString machineCode = getMachineCode();
	bool ok = false;
	auto info = rw::actCry::ActivationInfo::load(getRegistryCfg(), ok);
	return ok && info.hwid == machineCode.toStdString();
}

QString LicenseManager::getAuthorizationExpiry()
{
	return QStringLiteral("永久授权");
}

bool LicenseManager::applyActivationCode(const QString& activationCode)
{
	if (activationCode.isEmpty())
		return false;

	bool parseOk = false;
	auto info = rw::actCry::ActivationInfo::parseActivationCode(
		activationCode.toStdString(), kActivationKey, parseOk);
	if (!parseOk)
	{
		QMessageBox::warning(nullptr, "错误", "激活码无效或格式错误");
		return false;
	}

	const QString machineCode = getMachineCode();
	if (info.hwid != machineCode.toStdString())
	{
		QMessageBox::warning(nullptr, "错误",
			"激活码与当前机器不匹配。请确认机器码后重新申请激活码。");
		return false;
	}

	if (!rw::actCry::ActivationInfo::save(info, getRegistryCfg()))
	{
		QMessageBox::warning(nullptr, "错误", "保存授权信息失败，请检查权限或联系管理员");
		return false;
	}

	return true;
}

bool LicenseManager::verifyAtStartup()
{
	const QString machineCode = getMachineCode();
	qDebug() << "当前机器码:" << machineCode;

	// 仅校验当前是否已授权；未授权时的 DlgLicense 弹窗交互由 AppRuntime 处理
	// （BusinessLayer 不依赖 UI 层）
	return isAuthorized();
}

QString LicenseManager::getActivationKey()
{
	return QString::fromLatin1(kActivationKey);
}
