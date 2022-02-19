#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include "ui_Builder.h"
#include "MyOpenGLWidget.h"
#include "FuncTabWidget.h"
#include "TitleBar.h"
#include <windowsx.h>
#include "default.h"

class Builder : public QMainWindow
{
	Q_OBJECT

public:
	Builder(QWidget *parent = Q_NULLPTR);
	// 对指定的widget对象为其加载样式表
	static void loadStyleSheet(QWidget *widget, const QString& styleSheetFile);
public slots:
	// 打开一个文件对话框，获取模型文件的路径
	void openModelFileDialog();
	// 打开一个颜色选择对话框
	void openColorDialog();
	// 打开一个保存文件对话框，获取想要保存的文件路径
	void openSaveFileDialog();
	// 命令行调用渲染程序
	void callRender();
protected:
	// 处理windows native事件，实现无边框窗口的可拉伸
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
private:
	Ui::BuilderClass ui;
	// OpenGL窗口
	MyOpenGLWidget* openGLWidget;
	// 当前编辑器中的场景的文件路径，如没有保存则为空，保存过一次后即为当前路径
	// 用于保存时直接保存不再弹出路径选择对话框，另存为仍然会打开
	QString file_path;
	QPushButton* save_button;
	bool eventFilter(QObject* watched, QEvent* event) override;
	std::string ExeCmd(std::string pszCmd);
// QT信号，调用则激活与信号绑定的函数，函数参数为信号传递的参数
signals:
	// 发送模型文件的路径
	void loadModelPath(QString path);
	// 发送选择的颜色
	void colorSelected(QColor color);
	// 存储模型场景的路径
	void saveModels(QString path);
	// 发送信号通知开始渲染
	void startRender();
};
