#include "NumberKeyboard.h"

#include <QGuiApplication>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QShowEvent>
#include <QVBoxLayout>

NumberKeyboard::NumberKeyboard(QWidget* parent)
	: DialogDraggable(parent)
{
	build_ui();
}

NumberKeyboard::~NumberKeyboard() = default;

QString NumberKeyboard::getValue() const
{
	return _value;
}

void NumberKeyboard::showEvent(QShowEvent* event)
{
	DialogDraggable::showEvent(event);
	const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
	move(screenGeometry.center() - rect().center());
}

void NumberKeyboard::build_ui()
{
	setFixedSize(340, 418);
	// 无边框圆角效果：对话框本身透明，背景由内部容器控件绘制
	// （容器用普通 QWidget 实例而非子类，样式表背景才能被 style 正常绘制）
	setAttribute(Qt::WA_TranslucentBackground);

	auto* container = new QWidget(this);
	container->setObjectName(QStringLiteral("container"));
	container->setStyleSheet(QStringLiteral(R"(
QWidget#container {
	background-color: #f5f5f5;
	border-radius: 10px;
}
QLineEdit {
	background-color: white;
	border: 1px solid #d0d0d0;
	border-radius: 6px;
	font-size: 24px;
	padding: 0 8px;
}
QPushButton {
	background-color: #fcfcfc;
	border: 1px solid #cfcfcf;
	border-radius: 6px;
	font-size: 20px;
}
QPushButton:pressed {
	background-color: #dcdcdc;
}
QPushButton#pbtn_ok {
	background-color: #e6e6e6;
}
QPushButton#pbtn_ok:pressed {
	background-color: #d0d0d0;
}
)"));

	auto* outerLayout = new QVBoxLayout(this);
	outerLayout->setContentsMargins(0, 0, 0, 0);
	outerLayout->addWidget(container);

	auto* mainLayout = new QVBoxLayout(container);
	mainLayout->setContentsMargins(14, 14, 14, 14);
	mainLayout->setSpacing(10);

	_display = new QLineEdit(this);
	_display->setReadOnly(true);
	_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	_display->setFixedHeight(54);
	mainLayout->addWidget(_display);

	auto* gridLayout = new QGridLayout;
	gridLayout->setSpacing(10);
	mainLayout->addLayout(gridLayout);

	// 数字键 1-9（三行三列）
	for (int num = 1; num <= 9; ++num)
	{
		auto* button = new QPushButton(QString::number(num), this);
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(button, &QPushButton::clicked, this, [this, num]() { appendChar(QChar('0' + num)); });
		gridLayout->addWidget(button, (num - 1) / 3, (num - 1) % 3);
	}

	// 第四行：负号、0、小数点
	auto* pbtn_bar = new QPushButton(QStringLiteral("-"), this);
	pbtn_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_bar, &QPushButton::clicked, this, [this]()
		{
			// 负号切换：已有负号则去掉，否则加到最前
			if (_value.startsWith('-'))
				_value.remove(0, 1);
			else
				_value.prepend('-');
			_display->setText(_value);
		});
	gridLayout->addWidget(pbtn_bar, 3, 0);

	auto* pbtn_num0 = new QPushButton(QStringLiteral("0"), this);
	pbtn_num0->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_num0, &QPushButton::clicked, this, [this]() { appendChar(QChar('0')); });
	gridLayout->addWidget(pbtn_num0, 3, 1);

	auto* pbtn_point = new QPushButton(QStringLiteral("."), this);
	pbtn_point->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_point, &QPushButton::clicked, this, [this]()
		{
			if (!_value.contains('.'))
				appendChar(QChar('.'));
		});
	gridLayout->addWidget(pbtn_point, 3, 2);

	// 第五行：删除、取消、确定
	auto* pbtn_delete = new QPushButton(tr("删除"), this);
	pbtn_delete->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_delete, &QPushButton::clicked, this, [this]()
		{
			if (!_value.isEmpty())
			{
				_value.chop(1);
				_display->setText(_value);
			}
		});
	gridLayout->addWidget(pbtn_delete, 4, 0);

	auto* pbtn_cancel = new QPushButton(tr("取消"), this);
	pbtn_cancel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_cancel, &QPushButton::clicked, this, &QDialog::reject);
	gridLayout->addWidget(pbtn_cancel, 4, 1);

	auto* pbtn_ok = new QPushButton(tr("确定"), this);
	pbtn_ok->setObjectName(QStringLiteral("pbtn_ok"));
	pbtn_ok->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(pbtn_ok, &QPushButton::clicked, this, &QDialog::accept);
	gridLayout->addWidget(pbtn_ok, 4, 2);
}

void NumberKeyboard::appendChar(const QChar& ch)
{
	_value.append(ch);
	_display->setText(_value);
}
