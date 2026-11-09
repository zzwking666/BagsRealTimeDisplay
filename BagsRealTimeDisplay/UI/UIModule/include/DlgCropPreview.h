#pragma once

#include <QDialog>
#include <QPixmap>

class QLabel;
class QResizeEvent;

class DlgCropPreview : public QDialog
{
	Q_OBJECT

public:
	explicit DlgCropPreview(QWidget* parent = nullptr);

	void setSnapshot(const QPixmap& pixmap);

protected:
	void resizeEvent(QResizeEvent* e) override;

private:
	void updateScaledPixmap();

	QLabel* _label{ nullptr };
	QPixmap _snapshot{};
};
