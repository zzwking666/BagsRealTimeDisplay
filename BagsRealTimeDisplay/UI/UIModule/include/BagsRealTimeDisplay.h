#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BagsRealTimeDisplay.h"
#include <QTimer>
#include "rqw_LabelClickable.h"
#include "DlgCloseForm.h"
#include "ConfigModule.hpp"
#include "CameraModule.hpp"
#include "DlgProductSet.h"
#include "PanZoomLabel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class BagsRealTimeDisplayClass; };
QT_END_NAMESPACE

class BagsRealTimeDisplay : public QMainWindow
{
    Q_OBJECT

public:
    BagsRealTimeDisplay(ConfigModule& configModule,CameraModule& cameraModule,QWidget* parent = nullptr);
    ~BagsRealTimeDisplay();
public:
	void initializeComponents();
	/// 语言切换后重刷全部界面文本（含动态文本与子对话框）。
	/// 注意：不能调用 ui->retranslateUi —— label_imgDisplay_1 已被
	/// PanZoomLabel 替换并 deleteLater，retranslateUi 会访问悬垂指针。
	void retranslate();
public:
	void build_ui();
	void build_connect();
	void build_BagsRealTimeDisplayData();
    void build_DlgCloseForm();
	void build_DlgProductSet();
	void build_PanZoomLabel();
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	void touchManualViewOperation();
public slots:
	void updateCameraLabelState(int cameraIndex, bool state);

	void onUpdateStatisticalInfoUI();

    void onCameraDisplay(size_t index, const QImage& image);

	void setConfigWindowClosed();

	void onZMotionDisconnect();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void btn_jianshaobaoguang1_clicked();
	void btn_zengjiabaoguang1_clicked();
	void btn_jianshaobaoguang2_clicked();
	void btn_zengjiabaoguang2_clicked();
	void pbtn_resetProduct_clicked();
	void pbtn_setchishu_clicked();

	void cbb_qiehuanxianshi_currentIndexChanged(int index);

	void lb_title_clicked();
private:
	void setChishu(float chishu);
public:
	DlgProductSet* _dlgProductSet = nullptr;
private:
	rw::rqw::ClickableLabel* clickableTitle = nullptr;
	int minimizeCount{ 3 };
	DlgCloseForm* _dlgCloseForm = nullptr;

	// 正面背面切换锚点
	int lastCameraCaptureCount{ 0 };
	int lastCameraCaptureIndex{ 1 };

	// 每相机帧计数器：每两帧只显示一帧，降低显示刷新率
	unsigned int _frontFrameCounter{ 0 };
	unsigned int _backFrameCounter{ 0 };

	// 自定义可操作label
	PanZoomLabel* _panZoomLabel = nullptr;
	PanZoomLabel::ViewState _frontViewState{};
	PanZoomLabel::ViewState _backViewState{};
	int _currentViewCamera{ 0 };	// 1:正面相机, 2:背面相机

	// 手动操作后冻结图像更新
	bool _freezeImageUpdate{ false };
	int _freezeAfterManualMs{ 3000 };
	QTimer* _manualFreezeTimer{ nullptr };

	// ZMotion断开弹窗，最多连续弹3次
	int _zMotionWarningCount{ 0 };

	// 相机连接状态缓存，语言切换后按缓存重刷状态文本
	bool _camera1Connected{ false };
	bool _camera2Connected{ false };
private:
    Ui::BagsRealTimeDisplayClass* ui;
    ConfigModule& _configModule;
	CameraModule& _cameraModule;
};

