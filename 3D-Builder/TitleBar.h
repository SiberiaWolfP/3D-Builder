#pragma once
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include "default.h"
#include <QtCore/QEvent>
#include <QtCore/QPoint>

/**
 * \brief 自定义标题栏，可以更改颜色和功能按键的外观
 */
class TitleBar : public QWidget
{
private:
	// 标题栏文字
	QLabel *titleLabel;
	// 标题栏图标
	QLabel *iconLabel;
	// 最小化、最大化、关闭按钮
	QPushButton *minButton;
	QPushButton *restoreButton;
	QPushButton *closeButton;

	// 各控件的外层layout，将控件排列整齐
	QHBoxLayout *lay;

	// 父窗口指针
	QWidget *win;

	// 按钮是否按下（没有释放）
	bool isPressed;
	// 鼠标按下时的坐标，之后当鼠标拖动时不更新，用于拖动窗口
	QPoint startPos;

private:
	// 初始化widget
	void InitializeWidget();
	// 初始化各控件样式
	void InitializeViews();

private slots:
	// 由槽函数可激活最小化、最大化和关闭功能，其中Restore函数为双击时最大化或返回正常大小
	void ShowRestoreWindow();
	void ShowMaximizedWindow();
	void ShowMinimizedWindow();
	void CloseWindow();
	void loadStyleSheet(const QString& styleSheetFile);
public:
	// 对鼠标事件的操作
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

public:
	TitleBar(QWidget *parent = nullptr);
	virtual ~TitleBar();

public:
	// 可由外部调用设置标题栏文字和图标
	void SetTitle(const QString &str);
	void SetIcon(QPixmap &pix);
};

