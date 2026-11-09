#include "DlgCropPreview.h"

#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>

DlgCropPreview::DlgCropPreview(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(QStringLiteral("局部放大"));
	resize(520, 520);

	_label = new QLabel(this);
	_label->setAlignment(Qt::AlignCenter);
	_label->setScaledContents(false);
	_label->setAutoFillBackground(true);

	QPalette labelPalette = _label->palette();
	labelPalette.setColor(QPalette::Window, Qt::black);
	_label->setPalette(labelPalette);

	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(_label);
}

void DlgCropPreview::setSnapshot(const QPixmap& pixmap)
{
	_snapshot = pixmap;
	updateScaledPixmap();
}

void DlgCropPreview::resizeEvent(QResizeEvent* e)
{
	QDialog::resizeEvent(e);
	updateScaledPixmap();
}

void DlgCropPreview::updateScaledPixmap()
{
	if (!_label)
		return;

	if (_snapshot.isNull())
	{
		_label->clear();
		return;
	}

	if (_label->width() <= 0 || _label->height() <= 0)
		return;

	_label->setPixmap(_snapshot.scaled(_label->size(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
