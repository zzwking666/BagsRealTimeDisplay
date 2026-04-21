#include "BagsRealTimeDisplay.h"

BagsRealTimeDisplay::BagsRealTimeDisplay(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::BagsRealTimeDisplayClass())
{
	ui->setupUi(this);

	initializeComponents();
}

BagsRealTimeDisplay::~BagsRealTimeDisplay()
{
	
}

void BagsRealTimeDisplay::initializeComponents()
{
	build_ui();

	build_camera();

	build_connect();
}

void BagsRealTimeDisplay::build_ui()
{
	build_BagsRealTimeDisplayData();
	build_DlgCloseForm();
	ini_clickableTitle();
}

void BagsRealTimeDisplay::build_connect()
{
	QObject::connect(ui->pbtn_exit, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_exit_clicked);
	QObject::connect(ui->pbtn_set, &QPushButton::clicked,
		this, &BagsRealTimeDisplay::pbtn_set_clicked);
	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &BagsRealTimeDisplay::lb_title_clicked);
}

void BagsRealTimeDisplay::build_BagsRealTimeDisplayData()
{

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

void BagsRealTimeDisplay::build_camera()
{

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

}