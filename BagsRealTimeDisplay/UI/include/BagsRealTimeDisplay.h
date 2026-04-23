#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BagsRealTimeDisplay.h"

#include "rqw_LabelClickable.h"
#include "DlgCloseForm.h"
#include "DlgProductSet.h"

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
	void build_setConfig();
	void ini_clickableTitle();
	void build_DlgCloseForm();
public slots:
	void updateCameraLabelState(int cameraIndex, bool state);

    void onCameraDisplay(size_t index, QPixmap image);

	void setConfigWindowClosed();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void btn_jianshaobaoguang1_clicked();
	void btn_zengjiabaoguang1_clicked();
	void btn_jianshaobaoguang2_clicked();
	void btn_zengjiabaoguang2_clicked();

	void lb_title_clicked();
private:
	void loadCompanyTXT();
private:
	rw::rqw::ClickableLabel* clickableTitle = nullptr;
	int minimizeCount{ 3 };
	DlgCloseForm* _dlgCloseForm = nullptr;
	DlgProductSet* _dlgProductSet = nullptr;
private:
    Ui::BagsRealTimeDisplayClass* ui;
};

