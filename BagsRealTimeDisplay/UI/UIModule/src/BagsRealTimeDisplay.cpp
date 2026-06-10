#include "BagsRealTimeDisplay.h"
#include "DlgProductSet.h"

#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <rwul/hoepZMotion/hoepZMotion_ZMotionDevice.hpp>
#include <rwul/rqwu/Keyboard/rqwu_NumberKeyboard.h>
#include "rwul/rqwu/rqwu_MessageBox.h"

#include "Modules.hpp"
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
}

void BagsRealTimeDisplay::build_ui()
{
	build_BagsRealTimeDisplayData();
	build_DlgCloseForm();
	build_DlgProductSet();
	build_PanZoomLabel();
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
	QObject::connect(ui->pbtn_resetProduct, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_resetProduct_clicked);
	QObject::connect(ui->pbtn_setchishu, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_setchishu_clicked);

	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &BagsRealTimeDisplay::lb_title_clicked);
}

void BagsRealTimeDisplay::build_BagsRealTimeDisplayData()
{
	auto& BagsRealTimeDisplayConfig = _configModule.bagsRealTimeDisplayInfo;
	auto& setConfig = _configModule.setConfig;

	ui->cbb_qiehuanxianshi->addItems({ "正面", "背面", "双面" });

	// 更新UI
	ui->lb_FrontTotal->setText(QString::number(BagsRealTimeDisplayConfig.zhengmianzongliang));
	ui->lb_BackTotal->setText(QString::number(BagsRealTimeDisplayConfig.beimianzongliang));
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
	ui->cbb_qiehuanxianshi->setCurrentIndex(BagsRealTimeDisplayConfig.qiehuanxianshi);
	ui->pbtn_setchishu->setText(QString::number(BagsRealTimeDisplayConfig.shezhichishu));
}

void BagsRealTimeDisplay::build_DlgCloseForm()
{
	_dlgCloseForm = new DlgCloseForm(this);
}

void BagsRealTimeDisplay::build_DlgProductSet()
{
	_dlgProductSet = new DlgProductSet(_configModule, _cameraModule, this);
}

void BagsRealTimeDisplay::build_PanZoomLabel()
{
	if (_panZoomLabel) return;
	if (!ui || !ui->label_imgDisplay_1) return;

	QLabel* oldLabel = ui->label_imgDisplay_1;

	// 用原label同一个父对象，避免层级变化
	_panZoomLabel = new PanZoomLabel(oldLabel->parentWidget());

	// 迁移常用属性
	_panZoomLabel->setObjectName(oldLabel->objectName());
	_panZoomLabel->setGeometry(oldLabel->geometry());
	_panZoomLabel->setSizePolicy(oldLabel->sizePolicy());
	_panZoomLabel->setMinimumSize(oldLabel->minimumSize());
	_panZoomLabel->setMaximumSize(oldLabel->maximumSize());
	_panZoomLabel->setBaseSize(oldLabel->baseSize());
	_panZoomLabel->setContentsMargins(oldLabel->contentsMargins());

	_panZoomLabel->setStyleSheet(oldLabel->styleSheet());
	_panZoomLabel->setFont(oldLabel->font());
	_panZoomLabel->setCursor(oldLabel->cursor());
	_panZoomLabel->setEnabled(oldLabel->isEnabled());
	_panZoomLabel->setVisible(oldLabel->isVisible());

	// QLabel相关视觉属性
	_panZoomLabel->setAlignment(oldLabel->alignment());
	_panZoomLabel->setFrameShape(oldLabel->frameShape());
	_panZoomLabel->setFrameShadow(oldLabel->frameShadow());
	_panZoomLabel->setLineWidth(oldLabel->lineWidth());
	_panZoomLabel->setMidLineWidth(oldLabel->midLineWidth());
	_panZoomLabel->setIndent(oldLabel->indent());
	_panZoomLabel->setMargin(oldLabel->margin());

	// 用布局替换（首选）
	if (oldLabel->parentWidget() && oldLabel->parentWidget()->layout())
	{
		oldLabel->parentWidget()->layout()->replaceWidget(oldLabel, _panZoomLabel);
	}
	else
	{
		// 没有布局时，至少保持位置尺寸一致
		_panZoomLabel->move(oldLabel->pos());
		_panZoomLabel->resize(oldLabel->size());
	}

	_panZoomLabel->show();
	oldLabel->hide();
	oldLabel->deleteLater();

	_panZoomLabel->installEventFilter(this);

	// 冻结计时器
	if (!_manualFreezeTimer)
	{
		_manualFreezeTimer = new QTimer(this);
		_manualFreezeTimer->setSingleShot(true);
		connect(_manualFreezeTimer, &QTimer::timeout, this, [this]()
			{
				_freezeImageUpdate = false;
			});
	}
}

bool BagsRealTimeDisplay::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == _panZoomLabel && event)
	{
		switch (event->type())
		{
		case QEvent::Wheel:
			touchManualViewOperation();
			break;

		case QEvent::MouseButtonPress:
		{
			auto* me = static_cast<QMouseEvent*>(event);
			if (me->button() == Qt::LeftButton) touchManualViewOperation();
			break;
		}

		case QEvent::MouseMove:
		{
			auto* me = static_cast<QMouseEvent*>(event);
			if (me->buttons() & Qt::LeftButton) touchManualViewOperation();
			break;
		}

		default:
			break;
		}
	}

	return QMainWindow::eventFilter(watched, event);
}

void BagsRealTimeDisplay::touchManualViewOperation()
{
	_freezeImageUpdate = true;
	if (_manualFreezeTimer)
	{
		int stopTime = static_cast<int>(_configModule.setConfig.xuantingshijian * 1000);
		_manualFreezeTimer->start(stopTime);
	}
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

void BagsRealTimeDisplay::onUpdateStatisticalInfoUI()
{
	auto& _statisticalInfo = Modules::getInstance().asynchronousThreadModule.statisticalInfo;

	ui->lb_FrontTotal->setText(QString::number(_statisticalInfo.zhengmianzongliang));
	ui->lb_BackTotal->setText(QString::number(_statisticalInfo.beimianzongliang));
}

void BagsRealTimeDisplay::onZMotionDisconnect()
{
	rw::rqwu::MessageBox::warning(this,"警告！","运动控制器断开连接！");
}

void BagsRealTimeDisplay::onCameraDisplay(size_t index, const QImage& image)
{
	if (_freezeImageUpdate) return;
	if (!_panZoomLabel || image.isNull()) return;
	auto& _statisticalInfo = Modules::getInstance().asynchronousThreadModule.statisticalInfo;

	const int mode = _configModule.bagsRealTimeDisplayInfo.qiehuanxianshi;

	// 根据出图张数控制显示刷新率：每 N 帧只显示一帧
	const int chuTuZhangShu = _configModule.setConfig.chutuzhangshu;
	if (chuTuZhangShu > 1)
	{
		if (index == 1)
		{
			++_frontFrameCounter;
			if (_frontFrameCounter % chuTuZhangShu != 0) return;
		}
		else if (index == 2)
		{
			++_backFrameCounter;
			if (_backFrameCounter % chuTuZhangShu != 0) return;
		}
	}

	auto showByCamera = [&](int camera, const QPixmap& pix)
		{
			// 先保存上一相机视图
			if (_currentViewCamera == 1) _frontViewState = _panZoomLabel->viewState();
			if (_currentViewCamera == 2) _backViewState = _panZoomLabel->viewState();

			// 首次显示该相机图像时自动铺满，后续保持用户视图
			const bool isFirstShow = (camera == 1 && !_frontViewState.valid) || (camera == 2 && !_backViewState.valid);
			if (isFirstShow)
			{
				_panZoomLabel->setPixmap(pix);
			}
			else
			{
				_panZoomLabel->setPixmap(pix, false);
			}

			// 恢复当前相机各自视图
			if (camera == 1 && _frontViewState.valid) _panZoomLabel->applyViewState(_frontViewState);
			if (camera == 2 && _backViewState.valid)  _panZoomLabel->applyViewState(_backViewState);

			_currentViewCamera = camera;
		};

	if (mode == 0)
	{
		if (index == 1)
		{
			showByCamera(1, QPixmap::fromImage(image));
			++_statisticalInfo.zhengmianzongliang;
			return;
		}
	}
	if (mode == 1)
	{
		if (index == 2)
		{
			showByCamera(2, QPixmap::fromImage(image));
			++_statisticalInfo.beimianzongliang;
			return;
		}
	}

	if (mode == 2)
	{
		if (index == static_cast<size_t>(lastCameraCaptureIndex))
		{
			if (index == 1)
			{
				showByCamera(1, QPixmap::fromImage(image));
				++_statisticalInfo.zhengmianzongliang;
			}
			else
			{
				showByCamera(2, QPixmap::fromImage(image));
				++_statisticalInfo.beimianzongliang;
			}
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

void BagsRealTimeDisplay::pbtn_resetProduct_clicked()
{
	auto& _statisticalInfo = Modules::getInstance().asynchronousThreadModule.statisticalInfo;

	_statisticalInfo.zhengmianzongliang = 0;
	_statisticalInfo.beimianzongliang = 0;
}

void BagsRealTimeDisplay::pbtn_setchishu_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			QMessageBox::warning(this, "提示", "请输入大于等于0的数值");
			return;
		}
		ui->pbtn_setchishu->setText(QString::number(value.toDouble()));
		auto& bagsRealTimeDisplayInfo = _configModule.bagsRealTimeDisplayInfo;
		bagsRealTimeDisplayInfo.shezhichishu = value.toDouble();
		//  TODO: 通过 ZMotionModule 下发齿数参数到 Zmotion 控制器
		//  Modules::getInstance().zMotionModule.setGearRatio(value.toDouble());
	}
}

void BagsRealTimeDisplay::cbb_qiehuanxianshi_currentIndexChanged(int index)
{
	_configModule.bagsRealTimeDisplayInfo.qiehuanxianshi = index;
	lastCameraCaptureCount = 0; // 切换显示模式时重置计数器
	lastCameraCaptureIndex = 1; // 重置为默认相机索引
}
