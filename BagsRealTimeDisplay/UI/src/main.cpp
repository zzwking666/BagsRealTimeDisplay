#include <QFile>

#include "BagsRealTimeDisplay.h"
#include "Modules.hpp"
#include <QtWidgets/QApplication>
#include "rqwu/rqwu_core.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(BagsRealTimeDisplay);
    rw::rqwu::ini();

    // 检测运行环境
    if (!Modules::check())
    {
        return 1;
    }

    Modules::getInstance().build();
    BagsRealTimeDisplay w(Modules::getInstance().configModule, Modules::getInstance().cameraModule);
    Modules::getInstance().connect();
    Modules::getInstance().start();

#ifdef NDEBUG
    w.showFullScreen();
#else
    w.show();
#endif
    return app.exec();
}
