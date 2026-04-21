#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BagsRealTimeDisplay.h"

#include "rqw_LabelClickable.h"
#include "DlgCloseForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BagsRealTimeDisplayClass; };
QT_END_NAMESPACE

class BagsRealTimeDisplay : public QMainWindow
{
    Q_OBJECT

public:
    BagsRealTimeDisplay(QWidget* parent = nullptr);
    ~BagsRealTimeDisplay();
public:
	void initializeComponents();
public:
	void build_ui();
	void build_connect();
	void build_BagsRealTimeDisplayData();
	void ini_clickableTitle();
	void build_DlgCloseForm();
public:
	void build_camera();
public slots:
	void updateCameraLabelState(int cameraIndex, bool state);

    void onCameraDisplay(size_t index, QPixmap image);

	void lb_title_clicked();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void rbtn_debug_checked(bool checked);
	void rbtn_removeFunc_checked(bool checked);
private:
	rw::rqw::ClickableLabel* clickableTitle = nullptr;
	int minimizeCount{ 3 };
	DlgCloseForm* _dlgCloseForm = nullptr;
private:
    Ui::BagsRealTimeDisplayClass* ui;
};

