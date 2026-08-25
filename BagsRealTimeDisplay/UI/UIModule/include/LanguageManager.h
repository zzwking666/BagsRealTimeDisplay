#pragma once

#include <QObject>
#include <QTranslator>

/// 语言管理器：负责安装/卸载 QTranslator，并在语言切换时广播 languageChanged 信号。
/// 语言编码与 SetConfig::yuyan 对应：0 = 中文（源码内置语言，不加载翻译），1 = English。
class LanguageManager : public QObject
{
	Q_OBJECT

public:
	static LanguageManager& getInstance()
	{
		static LanguageManager instance;
		return instance;
	}

	LanguageManager(const LanguageManager&) = delete;
	LanguageManager& operator=(const LanguageManager&) = delete;

	/// 应用指定语言：0 中文 / 1 English，切换完成后发出 languageChanged
	void applyLanguage(int yuyan);

	int currentLanguage() const { return _currentLanguage; }

signals:
	void languageChanged(int yuyan);

private:
	LanguageManager() = default;
	~LanguageManager() override = default;

private:
	QTranslator* _translator{ nullptr };
	int _currentLanguage{ 0 };
};
