#include "BagsRealTimeDisplay.h"
#include "DlgProductSet.h"
#include <QFile>

#include "Modules.hpp"
#include "utility.hpp"

BagsRealTimeDisplay::BagsRealTimeDisplay(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::BagsRealTimeDisplayClass())
{
	ui->setupUi(this);

	initializeComponents();
}

BagsRealTimeDisplay::~BagsRealTimeDisplay()
{
	Modules::getInstance().stop();
	Modules::getInstance().destroy();
	delete ui;
}

void BagsRealTimeDisplay::initializeComponents()
{
	build_ui();

	build_connect();

	loadCompanyTXT();
}

void BagsRealTimeDisplay::build_ui()
{
	build_BagsRealTimeDisplayData();
	build_DlgCloseForm();
	ini_clickableTitle();
	build_setConfig();
}

void BagsRealTimeDisplay::build_connect()
{
	QObject::connect(ui->pbtn_exit, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_exit_clicked);
	QObject::connect(ui->pbtn_set, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_set_clicked);
	QObject::connect(ui->btn_jianshaobaoguang1, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::btn_jianshaobaoguang1_clicked);
	QObject::connect(ui->btn_zengjiabaoguang1, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::btn_zengjiabaoguang1_clicked);
	QObject::connect(ui->btn_jianshaobaoguang2, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::btn_jianshaobaoguang2_clicked);
	QObject::connect(ui->btn_zengjiabaoguang2, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::btn_zengjiabaoguang2_clicked);

	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &BagsRealTimeDisplay::lb_title_clicked);
}

void BagsRealTimeDisplay::build_BagsRealTimeDisplayData()
{
	auto& config = Modules::getInstance().configModule.bagsRealTimeDisplayInfo;

	// 更新UI
	ui->lb_FrontTotal->setText(QString::number(config.zhengmianzongliang));
	ui->lb_BackTotal->setText(QString::number(config.beimianzongliang));
	ui->btn_baoguang1->setText(QString::number(config.camera1Exposure));
	ui->btn_baoguang2->setText(QString::number(config.camera2Exposure));
}

void BagsRealTimeDisplay::build_setConfig()
{
	_dlgProductSet = new DlgProductSet(this);
}

void BagsRealTimeDisplay::ini_clickableTitle()
{
	// 初始化标题label
	clickableTitle = new rw::rqw::ClickableLabel(this);
	auto layoutTitle = ui->groupBox_head->layout();
	layoutTitle->replaceWidget(ui->label_title, clickableTitle);
	delete ui->label_title;
	clickableTitle->setText("编织袋实时显示");
	clickableTitle->setStyleSheet("QLabel {font-size: 30px;font-weight: bold;color: rgb(255, 255, 255);padding: 5px 5px;border-bottom: 2px solid #cccccc;}");
}

void BagsRealTimeDisplay::build_DlgCloseForm()
{
	_dlgCloseForm = new DlgCloseForm(this);
}

void BagsRealTimeDisplay::updateCameraLabelState(int cameraIndex, bool state)
{
	switch (cameraIndex)
	{
	case 1:
		if (state) {
			ui->label_camera1State->setText("连接成功");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);font-size: 18px;font - weight: bold;padding: 5px 5px;} "));
		}
		else {
			ui->label_camera1State->setText("连接失败");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);font-size: 18px;font - weight: bold;padding: 5px 5px;} "));
		}
		break;
	case 2:
		if (state) {
			ui->label_camera2State->setText("连接成功");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);font-size: 18px;font - weight: bold;padding: 5px 5px;} "));
		}
		else {
			ui->label_camera2State->setText("连接失败");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);font-size: 18px;font - weight: bold;padding: 5px 5px;} "));
		}
		break;
	default:
		break;
	}
}

void BagsRealTimeDisplay::onCameraDisplay(size_t index, QPixmap image)
{
	if (1 == index)
	{
		ui->label_imgDisplay_1->setPixmap(image.scaled(ui->label_imgDisplay_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
	else if (2 == index)
	{
		ui->label_imgDisplay_2->setPixmap(image.scaled(ui->label_imgDisplay_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}

void BagsRealTimeDisplay::lb_title_clicked()
{
	if (0 != minimizeCount)
	{
		minimizeCount--;
	}
	else if (0 >= minimizeCount)
	{
		// 最小化主窗体
		this->showMinimized();
		minimizeCount = 3; // 重置最小化计数器
	}
}

void BagsRealTimeDisplay::pbtn_exit_clicked()
{
#ifdef NDEBUG
	if (_dlgCloseForm)
	{
		_dlgCloseForm->exec();
	}
#else
	this->close();
#endif
}

void BagsRealTimeDisplay::pbtn_set_clicked()
{
	_dlgProductSet->setFixedSize(this->width(), this->height());
	_dlgProductSet->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_dlgProductSet->exec();
}

void BagsRealTimeDisplay::btn_jianshaobaoguang1_clicked()
{
	auto& config = Modules::getInstance().configModule.bagsRealTimeDisplayInfo;
	config.camera1Exposure -= 1;
	ui->btn_baoguang1->setText(QString::number(config.camera1Exposure));
	auto& cameraModule = Modules::getInstance().cameraModule;
	cameraModule.setCamera1ExposureTime(config.camera1Exposure);
}

void BagsRealTimeDisplay::btn_zengjiabaoguang1_clicked()
{
	auto& config = Modules::getInstance().configModule.bagsRealTimeDisplayInfo;
	config.camera1Exposure += 1;
	ui->btn_baoguang1->setText(QString::number(config.camera1Exposure));
	auto& cameraModule = Modules::getInstance().cameraModule;
	cameraModule.setCamera1ExposureTime(config.camera1Exposure);
}

void BagsRealTimeDisplay::btn_jianshaobaoguang2_clicked()
{
	auto& config = Modules::getInstance().configModule.bagsRealTimeDisplayInfo;
	config.camera2Exposure -= 1;
	ui->btn_baoguang2->setText(QString::number(config.camera2Exposure));
	auto& cameraModule = Modules::getInstance().cameraModule;
	cameraModule.setCamera2ExposureTime(config.camera2Exposure);
}

void BagsRealTimeDisplay::btn_zengjiabaoguang2_clicked()
{
	auto& config = Modules::getInstance().configModule.bagsRealTimeDisplayInfo;
	config.camera2Exposure += 1;
	ui->btn_baoguang2->setText(QString::number(config.camera2Exposure));
	auto& cameraModule = Modules::getInstance().cameraModule;
	cameraModule.setCamera2ExposureTime(config.camera2Exposure);
}

void BagsRealTimeDisplay::loadCompanyTXT()
{
	const QString companyRootPath = globalPath.companyTxtPath;
	QFile file(companyRootPath);
	if (!file.exists())
	{
		qWarning("company.txt不存在：%s", qUtf8Printable(companyRootPath));
		return;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qWarning("company.txt不存在：%s", qUtf8Printable(companyRootPath));
		return;
	}

	QTextStream in(&file);
	in.setEncoding(QStringConverter::Utf8);

	const QString content = in.readAll().trimmed();
	ui->label_companyInfo->setText(content);

	file.close();
}
