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
	// ��ָ����widget����Ϊ�������ʽ��
	static void loadStyleSheet(QWidget *widget, const QString& styleSheetFile);
public slots:
	// ��һ���ļ��Ի��򣬻�ȡģ���ļ���·��
	void openModelFileDialog();
	// ��һ����ɫѡ��Ի���
	void openColorDialog();
	// ��һ�������ļ��Ի��򣬻�ȡ��Ҫ������ļ�·��
	void openSaveFileDialog();
	// �����е�����Ⱦ����
	void callRender();
protected:
	// ����windows native�¼���ʵ���ޱ߿򴰿ڵĿ�����
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
private:
	Ui::BuilderClass ui;
	// OpenGL����
	MyOpenGLWidget* openGLWidget;
	// ��ǰ�༭���еĳ������ļ�·������û�б�����Ϊ�գ������һ�κ�Ϊ��ǰ·��
	// ���ڱ���ʱֱ�ӱ��治�ٵ���·��ѡ��Ի������Ϊ��Ȼ���
	QString file_path;
	QPushButton* save_button;
	bool eventFilter(QObject* watched, QEvent* event) override;
	std::string ExeCmd(std::string pszCmd);
// QT�źţ������򼤻����źŰ󶨵ĺ�������������Ϊ�źŴ��ݵĲ���
signals:
	// ����ģ���ļ���·��
	void loadModelPath(QString path);
	// ����ѡ�����ɫ
	void colorSelected(QColor color);
	// �洢ģ�ͳ�����·��
	void saveModels(QString path);
	// �����ź�֪ͨ��ʼ��Ⱦ
	void startRender();
};
