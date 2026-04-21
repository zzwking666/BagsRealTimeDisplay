#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BagsRealTimeDisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BagsRealTimeDisplay; };
QT_END_NAMESPACE

class BagsRealTimeDisplay : public QMainWindow
{
    Q_OBJECT

public:
    BagsRealTimeDisplay(QWidget* parent = nullptr);
    ~BagsRealTimeDisplay();

private:
    Ui::BagsRealTimeDisplay* ui;
};

