#include "TitleBar.h"

TitleBar::TitleBar(QWidget *parent)
	:QWidget(parent)
{
	QPalette pal(this->palette());
	this->setContentsMargins(0, 0, 0, 0);
	//设置背景色
	pal.setColor(QPalette::Background, QColor(APP_MAIN_COLOR));
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->win = parent;
	InitializeWidget();
}

TitleBar::~TitleBar()
{
}

void TitleBar::InitializeWidget()
{
	isPressed = false;
	setFixedHeight(TITLE_BAR_HEIGHT * screen_relative_y_pix);

	InitializeViews();
	loadStyleSheet(TITLE_STYLE);
}

void TitleBar::InitializeViews()
{
	// iconLabel = new QLabel(this);
	titleLabel = new QLabel(this);

	minButton = new QPushButton(this);
	restoreButton = new QPushButton(this);
	closeButton = new QPushButton(this);
	closeButton->setObjectName("closeButton");

	minButton->setFixedSize(TITLE_BUTTON_WIDTH * screen_relative_x_pix, TITLE_BUTTON_HEIGHT * screen_relative_y_pix);
	restoreButton->setFixedSize(TITLE_BUTTON_WIDTH * screen_relative_x_pix, TITLE_BUTTON_HEIGHT * screen_relative_y_pix);
	closeButton->setFixedSize(TITLE_BUTTON_WIDTH * screen_relative_x_pix, TITLE_BUTTON_HEIGHT * screen_relative_y_pix);
	// iconLabel->setFixedSize(TITLE_LABEL_SIZE, TITLE_LABEL_SIZE);
	titleLabel->setFixedHeight(TITLE_LABEL_SIZE * screen_relative_y_pix);

	// iconLabel->setAlignment(Qt::AlignLeft);
	titleLabel->setAlignment(Qt::AlignLeft);
	titleLabel->setAlignment(Qt::AlignVCenter);

	minButton->setIcon(QIcon(TITLE_MIN_ICON));
	restoreButton->setIcon(QIcon(TITLE_RESTORE_ICON));
	closeButton->setIcon(QIcon(TITLE_CLS_ICON));

	connect(minButton, &QPushButton::clicked, this, &TitleBar::ShowMinimizedWindow);
	connect(restoreButton, &QPushButton::clicked, this, &TitleBar::ShowRestoreWindow);
	connect(closeButton, &QPushButton::clicked, this, &TitleBar::CloseWindow);

	lay = new QHBoxLayout(this);
	this->setLayout(lay);
	lay->setSpacing(0);
	lay->setMargin(0);

	// lay->addWidget(iconLabel);
	lay->addWidget(titleLabel);
	lay->addWidget(minButton);
	lay->addWidget(restoreButton);
	lay->addWidget(closeButton);
}

void TitleBar::ShowMaximizedWindow()
{
	win->showMaximized();
}

void TitleBar::ShowMinimizedWindow()
{
	win->showMinimized();
}

void TitleBar::ShowRestoreWindow()
{
	if (win->isMaximized())
	{
		win->showNormal();
	}
	else
	{
		win->showMaximized();
	}
}

void TitleBar::CloseWindow()
{
	win->close();
}

void TitleBar::loadStyleSheet(const QString& styleSheetFile)
{
	QFile file(styleSheetFile);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QString::fromStdString(file.readAll().toStdString());//读取样式表文件
		styleSheet += tr("QLabel {margin-left: ") + QString::number(16 * screen_relative_x_pix) + tr("px;}");
		styleSheet += tr("QLabel {font-size: ") + QString::number(16 * screen_relative_x_pix) + tr("px;}");
		this->setStyleSheet(styleSheet);//把文件内容传参
		file.close();
	}
	else
	{
		QMessageBox::information(this, "tip", "cannot find qss file");
	}
}

void TitleBar::SetTitle(const QString &str)
{
	titleLabel->setText(str);
}

void TitleBar::SetIcon(QPixmap &pix)
{
	iconLabel->setPixmap(pix.scaled(iconLabel->size() - QSize(TITLE_ICON_MAG, TITLE_ICON_MAG)));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	ShowRestoreWindow();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
	/*if (win->isMaximized())
		return;*/

	isPressed = true;
	startPos = event->globalPos();

	return QWidget::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	isPressed = false;

	return QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (isPressed)
	{
		if (win->isMaximized())
		{
			win->showNormal();
		}

		QPoint movePos = event->globalPos() - startPos;
		startPos = event->globalPos();

		win->move(win->pos() + movePos);
	}

}