#include "LanguageManager.h"

#include <QCoreApplication>
#include <QDebug>

void LanguageManager::applyLanguage(int yuyan)
{
	if (yuyan == 1)
	{
		if (!_translator)
		{
			auto* translator = new QTranslator(this);
			if (translator->load(":/BagsRealTimeDisplay/i18n/BagsRealTimeDisplay_en.qm"))
			{
				_translator = translator;
				QCoreApplication::installTranslator(_translator);
			}
			else
			{
				qWarning() << "英文语言包加载失败，保持中文显示";
				delete translator;
				yuyan = 0;
			}
		}
	}
	else
	{
		yuyan = 0;
		if (_translator)
		{
			QCoreApplication::removeTranslator(_translator);
			delete _translator;
			_translator = nullptr;
		}
	}

	_currentLanguage = yuyan;
	emit languageChanged(_currentLanguage);
}
