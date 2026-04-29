#include "BagsRealTimeDisplay.h"
#include "DlgProductSet.h"

#include <QFile>
#include <QPainter>
#include "utility.hpp"

BagsRealTimeDisplay::BagsRealTimeDisplay(ConfigModule& configModule, CameraModule& cameraModule, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::BagsRealTimeDisplayClass())
	, _configModule(configModule)
	, _cameraModule(cameraModule)
{
	ui->setupUi(this);

	initializeComponents();
}

BagsRealTimeDisplay::~BagsRealTimeDisplay()
{
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
	build_DlgProductSet();
	ini_clickableTitle();
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
	QObject::connect(ui->cbb_qiehuanxianshi, &QComboBox::currentIndexChanged,
				this, &BagsRealTimeDisplay::cbb_qiehuanxianshi_currentIndexChanged);

	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &BagsRealTimeDisplay::lb_title_clicked);
}

void BagsRealTimeDisplay::build_BagsRealTimeDisplayData()
{
	auto& BagsRealTimeDisplayConfig = _configModule.bagsRealTimeDisplayInfo;
	auto& setConfig = _configModule.setConfig;

	ui->cbb_qiehuanxianshi->addItems({"正面", "背面", "双面"});

	// 更新UI
	ui->lb_FrontTotal->setText(QString::number(BagsRealTimeDisplayConfig.zhengmianzongliang));
	ui->lb_BackTotal->setText(QString::number(BagsRealTimeDisplayConfig.beimianzongliang));
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
	ui->cbb_qiehuanxianshi->setCurrentIndex(BagsRealTimeDisplayConfig.qiehuanxianshi);
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

void BagsRealTimeDisplay::build_DlgProductSet()
{
	_dlgProductSet = new DlgProductSet(_configModule, _cameraModule, this);
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

void BagsRealTimeDisplay::onCameraDisplay(size_t index, const QPixmap& image)
{
	const int mode = _configModule.bagsRealTimeDisplayInfo.qiehuanxianshi;
	const double youyijuli = _configModule.setConfig.youyijuli;
	const double suofangbili = _configModule.setConfig.suofang;

	auto showImage = [&](double offsetX, double scalePercent)
		{
			auto* label = ui->label_imgDisplay_1;
			const QSize dstSize = label->size();
			if (dstSize.isEmpty() || image.isNull()) return;

			// 100=原图，<100缩小，>100放大
			const double scale = std::max(0.01, scalePercent / 100.0);

			const int targetW = std::max(1, qRound(image.width() * scale));
			const int targetH = std::max(1, qRound(image.height() * scale));
			const QPixmap scaled = image.scaled(
				targetW, targetH,
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);

			QPixmap canvas(dstSize);
			canvas.fill(Qt::black);

			const int baseX = (dstSize.width() - scaled.width()) / 2;
			const int baseY = (dstSize.height() - scaled.height()) / 2;
			const int drawX = baseX + qRound(offsetX);

			QPainter painter(&canvas);
			painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
			painter.drawPixmap(drawX, baseY, scaled);

			label->setPixmap(canvas);
		};

	// 0: 正面(相机1) -> 不缩放
	if (mode == 0)
	{
		if (index == 1) showImage(0.0, 100.0);
		return;
	}

	// 1: 背面(相机2) -> 应用偏移+缩放
	if (mode == 1)
	{
		if (index == 2) showImage(youyijuli, suofangbili);
		return;
	}

	// 2: 双面切换
	if (mode == 2)
	{
		if (index == static_cast<size_t>(lastCameraCaptureIndex))
		{
			const bool isBack = (index == 2);
			showImage(isBack ? youyijuli : 0.0, isBack ? suofangbili : 100.0);

			++lastCameraCaptureCount;
			const int switchCount = _configModule.setConfig.qiehuanzhangshu;
			if (lastCameraCaptureCount >= switchCount)
			{
				lastCameraCaptureIndex = (lastCameraCaptureIndex == 1) ? 2 : 1;
				lastCameraCaptureCount = 0;
			}
		}
	}
}

void BagsRealTimeDisplay::setConfigWindowClosed()
{
	auto& setConfig = _configModule.setConfig;
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
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
	auto& setConfig = _configModule.setConfig;
	setConfig.baoguang1 -= 1;
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	auto& cameraModule = _cameraModule;
	cameraModule.setCamera1ExposureTime(setConfig.baoguang1);
}

void BagsRealTimeDisplay::btn_zengjiabaoguang1_clicked()
{
	auto& setConfig = _configModule.setConfig;
	setConfig.baoguang1 += 1;
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	auto& cameraModule = _cameraModule;
	cameraModule.setCamera1ExposureTime(setConfig.baoguang1);
}

void BagsRealTimeDisplay::btn_jianshaobaoguang2_clicked()
{
	auto& setConfig = _configModule.setConfig;
	setConfig.baoguang2 -= 1;
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
	auto& cameraModule = _cameraModule;
	cameraModule.setCamera2ExposureTime(setConfig.baoguang2);
}

void BagsRealTimeDisplay::btn_zengjiabaoguang2_clicked()
{
	auto& setConfig = _configModule.setConfig;
	setConfig.baoguang2 += 1;
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
	auto& cameraModule = _cameraModule;
	cameraModule.setCamera2ExposureTime(setConfig.baoguang2);
}

void BagsRealTimeDisplay::cbb_qiehuanxianshi_currentIndexChanged(int index)
{
	_configModule.bagsRealTimeDisplayInfo.qiehuanxianshi = index;
	lastCameraCaptureCount = 0; // 切换显示模式时重置计数器
	lastCameraCaptureIndex = 1; // 重置为默认相机索引
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
