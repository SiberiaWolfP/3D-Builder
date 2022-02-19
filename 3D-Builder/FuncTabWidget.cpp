#include "FuncTabWidget.h"

FuncTabWidget::FuncTabWidget(QWidget* parent) : QFrame(parent)
{
	loadStyleSheet(FUNC_TAB_WIDGET_STYLE);
	this->setContentsMargins(0, 0, 0, 0);
}

void FuncTabWidget::loadStyleSheet(const QString& styleSheetFile)
{
	QFile file(styleSheetFile);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QString::fromStdString(file.readAll().toStdString());//读取样式表文件
		this->setStyleSheet(styleSheet);//把文件内容传参
		file.close();
	}
	else
	{
		QMessageBox::information(this, "tip", "cannot find qss file");
	}
}
