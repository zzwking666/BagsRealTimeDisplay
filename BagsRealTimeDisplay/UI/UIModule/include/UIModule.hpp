#pragma once
#include "IModule.hpp"
#include<QObject>
#include "BagsRealTimeDisplay.h"

class UIModule
	:public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
public:
	BagsRealTimeDisplay _bagsRealTimeDisplay;
};
