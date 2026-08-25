#pragma once

#include <QCoreApplication>
#include <QMessageBox>

/// QMessageBox 标准按钮（Ok/Yes/No...）的文字在 Windows 下取自操作系统语言
/// （QWindowsTheme 读系统资源），不经过 QTranslator，无法随程序语言切换。
/// 这里用显式文本按钮替代标准按钮，使“确定”也能被翻译（英文模式显示 OK）。
namespace msgutil {

inline void execWithOkButton(QMessageBox& box)
{
	box.addButton(QCoreApplication::translate("MessageBoxUtil", "确定"), QMessageBox::AcceptRole);
	box.exec();
}

inline void warning(QWidget* parent, const QString& title, const QString& text)
{
	QMessageBox box(QMessageBox::Warning, title, text, QMessageBox::NoButton, parent);
	execWithOkButton(box);
}

inline void information(QWidget* parent, const QString& title, const QString& text)
{
	QMessageBox box(QMessageBox::Information, title, text, QMessageBox::NoButton, parent);
	execWithOkButton(box);
}

} // namespace msgutil
