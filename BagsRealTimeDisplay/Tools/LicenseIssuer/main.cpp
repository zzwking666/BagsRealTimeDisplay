#include "DlgLicenseIssuer.h"
#include "LicenseManager.hpp"

#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
	// CLI：打印本机机器码（与主程序 LicenseManager::getMachineCode 完全一致，
	// 复用同一套指纹算法，修复旧版 MaiLiDingZi 中产品名不一致的 bug）
	if (argc == 2 && std::strcmp(argv[1], "--machine-code") == 0)
	{
		QCoreApplication app(argc, argv);
		std::cout << LicenseManager::getMachineCode().toStdString() << std::endl;
		return 0;
	}

	QApplication app(argc, argv);
	DlgLicenseIssuer dlg;
	dlg.show();
	return app.exec();
}
