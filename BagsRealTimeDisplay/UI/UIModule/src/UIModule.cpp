#include "UIModule.hpp"

bool UIModule::build()
{
	return true;
}

void UIModule::destroy()
{
	_bagsRealTimeDisplay.close();
}

void UIModule::start()
{

}

void UIModule::stop()
{

}
