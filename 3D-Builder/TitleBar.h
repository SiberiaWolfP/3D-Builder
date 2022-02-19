#pragma once
#include <QtWidgets/QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include "default.h"
#include <QtCore/QEvent>
#include <QtCore/QPoint>

/**
 * \brief �Զ�������������Ը�����ɫ�͹��ܰ��������
 */
class TitleBar : public QWidget
{
private:
	// ����������
	QLabel *titleLabel;
	// ������ͼ��
	QLabel *iconLabel;
	// ��С������󻯡��رհ�ť
	QPushButton *minButton;
	QPushButton *restoreButton;
	QPushButton *closeButton;

	// ���ؼ������layout�����ؼ���������
	QHBoxLayout *lay;

	// ������ָ��
	QWidget *win;

	// ��ť�Ƿ��£�û���ͷţ�
	bool isPressed;
	// ��갴��ʱ�����֮꣬������϶�ʱ�����£������϶�����
	QPoint startPos;

private:
	// ��ʼ��widget
	void InitializeWidget();
	// ��ʼ�����ؼ���ʽ
	void InitializeViews();

private slots:
	// �ɲۺ����ɼ�����С������󻯺͹رչ��ܣ�����Restore����Ϊ˫��ʱ��󻯻򷵻�������С
	void ShowRestoreWindow();
	void ShowMaximizedWindow();
	void ShowMinimizedWindow();
	void CloseWindow();
	void loadStyleSheet(const QString& styleSheetFile);
public:
	// ������¼��Ĳ���
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

public:
	TitleBar(QWidget *parent = nullptr);
	virtual ~TitleBar();

public:
	// �����ⲿ�������ñ��������ֺ�ͼ��
	void SetTitle(const QString &str);
	void SetIcon(QPixmap &pix);
};

