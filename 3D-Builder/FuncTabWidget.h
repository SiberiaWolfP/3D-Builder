#pragma once
#include <QtWidgets/QtWidgets>
#include "default.h"

/**
 * \brief 每个Tab页下的狭长widget，只做了一些加载样式表的操作
 */
class FuncTabWidget : public QFrame
{
	Q_OBJECT
public:
	FuncTabWidget(QWidget* parent = Q_NULLPTR);
	void loadStyleSheet(const QString& styleSheetFile);
};
