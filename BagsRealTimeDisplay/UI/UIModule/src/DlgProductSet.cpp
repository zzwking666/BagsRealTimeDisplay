#include "DlgProductSet.h"
#include "ui_DlgProductSet.h"

#include <QMessageBox>
#include "MessageBoxUtil.hpp"
#include "rwul/rqwu/Keyboard/rqwu_NumberKeyboard.h"
#include "Modules.hpp"

DlgProductSet::DlgProductSet(ConfigModule& configModule, CameraModule& cameraModule, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductSetClass())
	, _configModule(configModule)
	, _cameraModule(cameraModule)
{
	ui->setupUi(this);

	build_ui();

	build_connect();
}

DlgProductSet::~DlgProductSet()
{
	delete ui;
}

void DlgProductSet::build_ui()
{
	read_config();
}

void DlgProductSet::read_config()
{
	auto& setConfig = _configModule.setConfig;

	ui->btn_chengfaqi1->setText(QString::number(setConfig.chengfaqi1));
	ui->btn_houfenpin1->setText(QString::number(setConfig.houfenpin1));
	ui->btn_zengyi1->setText(QString::number(setConfig.zengyi1));
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->btn_xiangsudangliang1->setText(QString::number(setConfig.xiangsudangliang1));

	ui->btn_chengfaqi2->setText(QString::number(setConfig.chengfaqi2));
	ui->btn_houfenpin2->setText(QString::number(setConfig.houfenpin2));
	ui->btn_zengyi2->setText(QString::number(setConfig.zengyi2));
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
	ui->btn_xiangsudangliang2->setText(QString::number(setConfig.xiangsudangliang2));

	ui->btn_xuantingshijian->setText(QString::number(setConfig.xuantingshijian));
	ui->btn_qiehuanzhangshu->setText(QString::number(setConfig.qiehuanzhangshu));
	ui->btn_chutuzhangshu->setText(QString::number(setConfig.chutuzhangshu));
	ui->ckb_jingxiang->setChecked(setConfig.isjingxiang);
	ui->cbb_yuyan->setCurrentIndex(setConfig.yuyan);

	// 丢帧默认为0
	ui->btn_xuantingshijian->setText(QString::number(setConfig.xuantingshijian));
	ui->btn_xuantingshijian->setText(QString::number(setConfig.xuantingshijian));
}

void DlgProductSet::build_connect()
{
	QObject::connect(ui->btn_close, &QPushButton::clicked,
		this, &DlgProductSet::btn_close_clicked);
	QObject::connect(ui->btn_chengfaqi1, &QPushButton::clicked,
		this, &DlgProductSet::btn_chengfaqi1_clicked);
	QObject::connect(ui->btn_houfenpin1, &QPushButton::clicked,
		this, &DlgProductSet::btn_houfenpin1_clicked);
	QObject::connect(ui->btn_zengyi1, &QPushButton::clicked,
		this, &DlgProductSet::btn_zengyi1_clicked);
	QObject::connect(ui->btn_baoguang1, &QPushButton::clicked,
		this, &DlgProductSet::btn_baoguang1_clicked);
	QObject::connect(ui->btn_xiangsudangliang1, &QPushButton::clicked,
		this, &DlgProductSet::btn_xiangsudangliang1_clicked);
	QObject::connect(ui->btn_chengfaqi2, &QPushButton::clicked,
		this, &DlgProductSet::btn_chengfaqi2_clicked);
	QObject::connect(ui->btn_houfenpin2, &QPushButton::clicked,
		this, &DlgProductSet::btn_houfenpin2_clicked);
	QObject::connect(ui->btn_zengyi2, &QPushButton::clicked,
		this, &DlgProductSet::btn_zengyi2_clicked);
	QObject::connect(ui->btn_baoguang2, &QPushButton::clicked,
		this, &DlgProductSet::btn_baoguang2_clicked);
	QObject::connect(ui->btn_xiangsudangliang2, &QPushButton::clicked,
		this, &DlgProductSet::btn_xiangsudangliang2_clicked);
	QObject::connect(ui->btn_xuantingshijian, &QPushButton::clicked,
		this, &DlgProductSet::btn_xuantingshijian_clicked);
	QObject::connect(ui->btn_qiehuanzhangshu, &QPushButton::clicked,
		this, &DlgProductSet::btn_qiehuanzhangshu_clicked);
	QObject::connect(ui->btn_chutuzhangshu, &QPushButton::clicked,
		this, &DlgProductSet::btn_chutuzhangshu_clicked);
	QObject::connect(ui->ckb_jingxiang, &QCheckBox::toggled,
		this, &DlgProductSet::ckb_jingxiang_toggled);
	QObject::connect(ui->cbb_yuyan, &QComboBox::currentIndexChanged,
		this, &DlgProductSet::cbb_yuyan_currentIndexChanged);
}

void DlgProductSet::retranslate()
{
	ui->retranslateUi(this);

	// retranslateUi 会把所有按钮重置为 .ui 占位文本，重新刷回配置值
	read_config();
	onUpdateFrameLost();
}

void DlgProductSet::showEvent(QShowEvent* event)
{
	QDialog::showEvent(event);

	auto& setConfig = _configModule.setConfig;
	ui->btn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->btn_baoguang2->setText(QString::number(setConfig.baoguang2));
}

void DlgProductSet::onUpdateFrameLost()
{
	auto& statisticInfo = Modules::getInstance().asynchronousThreadModule.statisticalInfo;
	ui->btn_diuzhen1->setText(QString::number(statisticInfo.cam1FrameLost));
	ui->btn_diuzhen2->setText(QString::number(statisticInfo.cam2FrameLost));
}

void DlgProductSet::btn_close_clicked()
{
	emit paramsChanged();
	this->close();
}

void DlgProductSet::btn_chengfaqi1_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_chengfaqi1->setText(value);
		setConfig.chengfaqi1 = value.toDouble();
		_cameraModule.setCamera1Multiplier(setConfig.chengfaqi1);
	}
}

void DlgProductSet::btn_houfenpin1_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_houfenpin1->setText(value);
		setConfig.houfenpin1 = value.toDouble();
		_cameraModule.setCamera1PostDivider(setConfig.houfenpin1);
	}
}

void DlgProductSet::btn_zengyi1_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_zengyi1->setText(value);
		setConfig.zengyi1 = value.toDouble();
		_cameraModule.setCamera1Gain(setConfig.zengyi1);
	}
}

void DlgProductSet::btn_baoguang1_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_baoguang1->setText(value);
		setConfig.baoguang1 = value.toDouble();
		_cameraModule.setCamera1ExposureTime(setConfig.baoguang1);
	}
}

void DlgProductSet::btn_xiangsudangliang1_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_xiangsudangliang1->setText(value);
		setConfig.xiangsudangliang1 = value.toDouble();
	}
}

void DlgProductSet::btn_chengfaqi2_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_chengfaqi2->setText(value);
		setConfig.chengfaqi2 = value.toDouble();
		_cameraModule.setCamera2Multiplier(setConfig.chengfaqi2);
	}
}

void DlgProductSet::btn_houfenpin2_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_houfenpin2->setText(value);
		setConfig.houfenpin2 = value.toDouble();
		_cameraModule.setCamera2PostDivider(setConfig.houfenpin2);
	}
}

void DlgProductSet::btn_zengyi2_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_zengyi2->setText(value);
		setConfig.zengyi2 = value.toDouble();
		_cameraModule.setCamera2Gain(setConfig.zengyi2);
	}
}

void DlgProductSet::btn_baoguang2_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_baoguang2->setText(value);
		setConfig.baoguang2 = value.toDouble();
		_cameraModule.setCamera2ExposureTime(setConfig.baoguang2);
	}
}

void DlgProductSet::btn_xiangsudangliang2_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_xiangsudangliang2->setText(value);
		setConfig.xiangsudangliang2 = value.toDouble();
	}
}

void DlgProductSet::btn_xuantingshijian_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_xuantingshijian->setText(value);
		setConfig.xuantingshijian = value.toDouble();
	}
}

void DlgProductSet::btn_qiehuanzhangshu_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_qiehuanzhangshu->setText(value);
		setConfig.qiehuanzhangshu = value.toDouble();
	}
}

void DlgProductSet::btn_chutuzhangshu_clicked()
{
	rw::rqwu::NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			msgutil::warning(this, tr("提示"), tr("请输入大于等于0的数值"));
			return;
		}
		auto& setConfig = _configModule.setConfig;
		ui->btn_chutuzhangshu->setText(value);
		setConfig.chutuzhangshu = value.toDouble();
	}
}

void DlgProductSet::ckb_jingxiang_toggled(bool checked)
{
	auto& setConfig = _configModule.setConfig;
	setConfig.isjingxiang = checked;
}

void DlgProductSet::cbb_yuyan_currentIndexChanged(int index)
{
	auto& setConfig = _configModule.setConfig;
	if (setConfig.yuyan == index) return;

	setConfig.yuyan = index;
	emit emit_changeLanguage(index);
}
