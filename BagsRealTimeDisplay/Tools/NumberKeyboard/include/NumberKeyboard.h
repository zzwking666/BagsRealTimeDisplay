#pragma once

#include <QString>

#include <rwul/rqwu/rqwu_DialogDraggable.h>

class QLineEdit;

/// 数字键盘控件，外观与行为复刻自 RWUL 的 rw::rqwu::NumberKeyboard
/// （库内按钮文字写死中文、不经过 QTranslator，无法随程序语言切换，故在 Tools 层
/// 重新实现可翻译版本）。界面文字均经 tr() 处理，新增/修改文字时需同步维护
/// UI/UIModule/translations/BagsRealTimeDisplay_en.ts 的 NumberKeyboard context。
class NumberKeyboard : public rw::rqwu::DialogDraggable
{
	Q_OBJECT
public:
	NumberKeyboard(QWidget* parent = nullptr);
	~NumberKeyboard() override;

	/// 获取当前输入值（字符串形式）。
	QString getValue() const;

protected:
	/// 弹出时将对话框居中到屏幕。
	void showEvent(QShowEvent* event) override;

private:
	void build_ui();
	void appendChar(const QChar& ch);

	QLineEdit* _display{ nullptr };
	QString _value;
};
