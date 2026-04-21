#include "BagsRealTimeDisplay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BagsRealTimeDisplay window;
    window.show();
    return app.exec();
}
