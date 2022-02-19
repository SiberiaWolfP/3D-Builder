#include "Builder.h"

Builder::Builder(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// ��ȡ��Ļ��С����������꣬��ƥ�䲻ͬ��С����Ļ
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QSize screen_rect = desktopWidget->screenGeometry().size();
	screen_relative_x_pix = screen_rect.width() / 1920.0f;
	screen_relative_y_pix = screen_rect.height() / 1080.0f;
	
	// ��Ļ��С
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMinimumWidth(screen_relative_x_pix * 400);
	this->setMinimumHeight(screen_relative_y_pix * 300);

	this->resize(screen_relative_x_pix * 1000, screen_relative_y_pix * 800);

	// �Զ��������
	TitleBar* title_bar = new TitleBar(this);
	title_bar->setMinimumHeight(screen_relative_y_pix * 40);
	title_bar->setMaximumHeight(screen_relative_y_pix * 40);
	title_bar->SetTitle("3D-Builder");

	// �����˵���ť
	QPushButton* menu_button = new QPushButton(this);
	menu_button->setIcon(QIcon(MENU_BTN_ICON));
	menu_button->setIconSize(QSize(screen_relative_x_pix * 62, screen_relative_y_pix * 60));

	// �Ҳ����Ϊ��ť
	QPushButton* save_as_button = new QPushButton(this);
	save_as_button->setObjectName("save_as_btn");
	save_as_button->setToolTip(QString::fromUtf8("���Ϊ"));
	save_as_button->setIcon(QIcon(SAVE_AS_BTN_ICON));
	save_as_button->setIconSize(QSize(screen_relative_x_pix * 30, screen_relative_y_pix * 30));
	
	// �Ҳౣ�水ť
	save_button = new QPushButton(this);
	save_button->setObjectName("save_btn");
	save_button->setToolTip(QString::fromUtf8("����"));
	save_button->setIcon(QIcon(SAVE_BTN_ICON));
	save_button->setIconSize(QSize(screen_relative_x_pix * 30, screen_relative_y_pix * 30));

	// �Ҳ���Ⱦ��ť
	QPushButton* render_button = new QPushButton(this);
	render_button->setIcon(QIcon(RENDER_BTN_ICON));
	render_button->setIconSize(QSize(screen_relative_x_pix * 30, screen_relative_y_pix * 30));

	QFrame* btn_group = new QFrame(this);
	QHBoxLayout* btn_group_layout = new QHBoxLayout(btn_group);
	btn_group_layout->addWidget(save_as_button);
	btn_group_layout->addWidget(save_button);
	btn_group_layout->addWidget(render_button);
	btn_group_layout->setContentsMargins(0, 0, 0, 0);
	
	// ����һ����ť
	QTabWidget* tab_widget = new QTabWidget(this);
	QPalette pal(tab_widget->palette());
	//���ñ���ɫ
	pal.setColor(QPalette::Background, QColor(APP_MAIN_COLOR));
	tab_widget->setAutoFillBackground(true);
	tab_widget->setPalette(pal);
	// ��Tab�������һ��menu��ť
	tab_widget->setCornerWidget(menu_button, Qt::TopLeftCorner);
	tab_widget->setCornerWidget(btn_group, Qt::TopRightCorner);
	// ��tab��Ӹ��Ե�widget
	tab_widget->addTab(new FuncTabWidget(this), "Insert");
	tab_widget->addTab(new FuncTabWidget(this), "Object");
	tab_widget->addTab(new FuncTabWidget(this), "Edit");
	tab_widget->addTab(new FuncTabWidget(this), "Paint");
	tab_widget->addTab(new FuncTabWidget(this), "View");
	tab_widget->addTab(new FuncTabWidget(this), "Debug");
	
	openGLWidget = new MyOpenGLWidget(this);
	// openGLWidget->installEventFilter(this);

	// ��Tab��ҳ����Ӱ�ť
	{
		QSize icon_size(screen_relative_x_pix * 24, screen_relative_y_pix * 24);
		// �����ҳ��Ӱ�ť
		{
			QHBoxLayout* layout = new QHBoxLayout(tab_widget->widget(0));
			// ����ģ�Ͱ�ť
			QPushButton* load_model_button = new QPushButton(tab_widget->widget(0));
			load_model_button->setIcon(QIcon(INSERT_BTN_ICON));
			load_model_button->setIconSize(icon_size);
			load_model_button->setText("Insert");
			load_model_button->setFixedWidth(screen_relative_x_pix * 90);
			// ����ڽ�ģ�Ͱ�ť
			QPushButton* cube_button = new QPushButton(tab_widget->widget(0));
			cube_button->setObjectName("cube_btn");
			cube_button->setIcon(QIcon(CUBE_BTN_ICON));
			cube_button->setIconSize(icon_size);
			QPushButton* cylinder_button = new QPushButton(tab_widget->widget(0));
			cylinder_button->setObjectName("cylinder_btn");
			cylinder_button->setIcon(QIcon(CYLINDER_BTN_ICON));
			cylinder_button->setIconSize(icon_size);
			QPushButton* pyramid_button = new QPushButton(tab_widget->widget(0));
			pyramid_button->setObjectName("pyramid_btn");
			pyramid_button->setIcon(QIcon(PYRAMID_BTN_ICON));
			pyramid_button->setIconSize(icon_size);
			QPushButton* ball_button = new QPushButton(tab_widget->widget(0));
			ball_button->setObjectName("ball_btn");
			ball_button->setIcon(QIcon(BALL_BTN_ICON));
			ball_button->setIconSize(icon_size);
			QPushButton* cone_button = new QPushButton(tab_widget->widget(0));
			cone_button->setObjectName("cone_btn");
			cone_button->setIcon(QIcon(CONE_BTN_ICON));
			cone_button->setIconSize(icon_size);
			QPushButton* hexagon_button = new QPushButton(tab_widget->widget(0));
			hexagon_button->setObjectName("hexagon_btn");
			hexagon_button->setIcon(QIcon(HEXAGON_BTN_ICON));
			hexagon_button->setIconSize(icon_size);
			QPushButton* ring_button = new QPushButton(tab_widget->widget(0));
			ring_button->setObjectName("ring_btn");
			ring_button->setIcon(QIcon(RING_BTN_ICON));
			ring_button->setIconSize(icon_size);
			QPushButton* tetrahedron_button = new QPushButton(tab_widget->widget(0));
			tetrahedron_button->setObjectName("tetrahedron_btn");
			tetrahedron_button->setIcon(QIcon(TETRAHEDRON_BTN_ICON));
			tetrahedron_button->setIconSize(icon_size);
			QPushButton* rabbit_button = new QPushButton(tab_widget->widget(0));
			rabbit_button->setObjectName("rabbit_btn");
			rabbit_button->setIcon(QIcon(RABBIT_BTN_ICON));
			rabbit_button->setIconSize(icon_size);
			
			layout->addWidget(load_model_button);
			// �ؼ��ָ������������հ׺��м�Ĵ�ֱ��
			layout->addSpacing(screen_relative_x_pix * 20);
			QPushButton* v_line = new QPushButton(tab_widget->widget(0));
			v_line->setFixedWidth(screen_relative_x_pix * 1);
			v_line->setFixedHeight(screen_relative_y_pix * 25);
			v_line->setStyleSheet("background-color:rgb(136, 136, 136);");
			v_line->setDisabled(true);
			layout->addWidget(v_line);
			layout->addSpacing(screen_relative_x_pix * 20);
			
			layout->addWidget(cube_button);
			layout->addWidget(cylinder_button);
			layout->addWidget(pyramid_button);
			layout->addWidget(cone_button);
			layout->addWidget(ball_button);
			layout->addWidget(hexagon_button);
			layout->addWidget(ring_button);
			layout->addWidget(tetrahedron_button);
			layout->addWidget(rabbit_button);
			layout->addStretch();
			layout->setSpacing(0);
			layout->setMargin(0);
			// ������ť�����Ӧ�Ĺ��ܺ�������
			connect(load_model_button, SIGNAL(clicked()), this, SLOT(openModelFileDialog()));
			connect(this, SIGNAL(loadModelPath(QString)), openGLWidget, SLOT(loadModel(QString)));
			connect(cube_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(cylinder_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(pyramid_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(ball_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(cone_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(hexagon_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(ring_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(tetrahedron_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
			connect(rabbit_button, SIGNAL(clicked()), openGLWidget, SLOT(insertInsideModel()));
		}
		// �����ҳ���밴ť
		{
			QHBoxLayout* layout = new QHBoxLayout(tab_widget->widget(1));
			// ����ѡ��ť
			QPushButton* select_button = new QPushButton(tab_widget->widget(1));
			select_button->setObjectName("select_obj_btn");
			select_button->setIcon(QIcon(OBJECT_SELECT_BTN_ICON));
			select_button->setIconSize(icon_size);
			select_button->setToolTip(QString::fromUtf8("ѡ��һ�������ٴ�ѡ��ȡ��"));
			select_button->setCheckable(true);
			select_button->setChecked(true);
			// ��ѡ��ť
			QPushButton* select_point_button = new QPushButton(tab_widget->widget(1));
			select_point_button->setObjectName("select_point_btn");
			select_point_button->setIcon(QIcon(POINT_SELECT_BTN_ICON));
			select_point_button->setIconSize(icon_size);
			select_point_button->setToolTip(QString::fromUtf8("ѡ�������ϵ�һ�����㣬�رչ��ܺ����"));
			select_point_button->setCheckable(true);

			layout->addWidget(select_button);
			layout->addWidget(select_point_button);
			layout->addStretch();
			layout->setSpacing(0);
			layout->setMargin(0);
			connect(select_button, SIGNAL(toggled(bool)), openGLWidget, SLOT(setFuncAble(bool)));
			connect(select_point_button, SIGNAL(toggled(bool)), openGLWidget, SLOT(setFuncAble(bool)));
		}
		// ��Viewҳ���밴ť
		{
			QHBoxLayout* layout = new QHBoxLayout(tab_widget->widget(4));
			// �߿�ģʽ����
			QPushButton* wire_frame_button = new QPushButton(tab_widget->widget(4));
			wire_frame_button->setObjectName("wire_frame_btn");
			wire_frame_button->setIcon(QIcon(WIRE_FRAME_BTN_ICON));
			wire_frame_button->setIconSize(icon_size);
			wire_frame_button->setToolTip(QString::fromUtf8("������رն����߿�Ļ���"));
			wire_frame_button->setCheckable(true);

			layout->addWidget(wire_frame_button);
			layout->addStretch();
			layout->setSpacing(0);
			layout->setMargin(0);
			connect(wire_frame_button, SIGNAL(toggled(bool)), openGLWidget, SLOT(setFuncAble(bool)));
		}
		// ���û�ͼҳ�ؼ�
		{
			QWidget* widget_paint = tab_widget->widget(3);
			QHBoxLayout* layout = new QHBoxLayout(widget_paint);

			QPushButton* color_select_button = new QPushButton(widget_paint);
			color_select_button->setObjectName("color_select_btn");
			color_select_button->setIcon(QIcon(SELECT_COLOR_BTN_ICON));
			color_select_button->setIconSize(icon_size);
			color_select_button->setToolTip(QString::fromUtf8("ѡ��ǰ��ѡ���������ɫ"));

			QLabel* label_color_alpha = new QLabel(widget_paint);
			label_color_alpha->setText("��͸����");

			QSlider* color_alpha_slider = new QSlider(widget_paint);
			color_alpha_slider->setObjectName("color_alpha_slider");
			color_alpha_slider->setMinimum(4000);
			color_alpha_slider->setMaximum(10000);
			color_alpha_slider->setValue(10000);
			color_alpha_slider->setOrientation(Qt::Horizontal);
			color_alpha_slider->setFixedWidth(200 * screen_relative_x_pix);
			color_alpha_slider->installEventFilter(this);

			QLabel* label_reflectivity = new QLabel(widget_paint);
			label_reflectivity->setText("�����");

			QSlider* reflectivity_slider = new QSlider(widget_paint);
			reflectivity_slider->setObjectName("reflectivity_slider");
			reflectivity_slider->setMinimum(0);
			reflectivity_slider->setMaximum(10000);
			reflectivity_slider->setOrientation(Qt::Horizontal);
			reflectivity_slider->setFixedWidth(200 * screen_relative_x_pix);
			reflectivity_slider->installEventFilter(this);

			layout->addWidget(color_select_button);
			layout->addSpacing(screen_relative_x_pix * 10);
			layout->addWidget(label_color_alpha);
			layout->addSpacing(screen_relative_x_pix * 10);
			layout->addWidget(color_alpha_slider);
			layout->addSpacing(screen_relative_x_pix * 10);
			layout->addWidget(label_reflectivity);
			layout->addSpacing(screen_relative_x_pix * 10);
			layout->addWidget(reflectivity_slider);
			layout->addStretch();
			layout->setSpacing(0);
			layout->setMargin(0);
			connect(color_select_button, SIGNAL(clicked()), this, SLOT(openColorDialog()));
			connect(this, SIGNAL(colorSelected(QColor)), openGLWidget, SLOT(colorSelected(QColor)));
			connect(color_alpha_slider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(alphaChanged(int)));
			connect(reflectivity_slider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(reflectivityChanged(int)));
		}
		// ����Debugҳ�ؼ�
		{
			QWidget* widget_debug = tab_widget->widget(5);
			QHBoxLayout* layout_debug = new QHBoxLayout(widget_debug);

			QLabel* label_ambient_debug = new QLabel(widget_debug);
			label_ambient_debug->setText("������ǿ�ȣ�");
			QDoubleSpinBox* spin_box_ambient_debug = new QDoubleSpinBox(widget_debug);
			spin_box_ambient_debug->setRange(0.0l, 5.0l);
			spin_box_ambient_debug->setSingleStep(0.01l);
			spin_box_ambient_debug->setDecimals(2);
			spin_box_ambient_debug->setValue(0.7l);

			QLabel* label_diffuse_debug = new QLabel(widget_debug);
			label_diffuse_debug->setText("������ǿ�ȣ�");
			QDoubleSpinBox* spin_box_diffuse_debug = new QDoubleSpinBox(widget_debug);
			spin_box_diffuse_debug->setRange(0.0l, 5.0l);
			spin_box_diffuse_debug->setSingleStep(0.01l);
			spin_box_diffuse_debug->setDecimals(2);
			spin_box_diffuse_debug->setValue(0.2l);

			QLabel* label_specular_debug = new QLabel(widget_debug);
			label_specular_debug->setText("���淴��ǿ�ȣ�");
			QDoubleSpinBox* spin_box_specular_debug = new QDoubleSpinBox(widget_debug);
			spin_box_specular_debug->setRange(0.0l, 5.0l);
			spin_box_specular_debug->setSingleStep(0.01l);
			spin_box_specular_debug->setDecimals(2);
			spin_box_specular_debug->setValue(0.1l);

			// �߿�ģʽ����
			QPushButton* pick_debug_button = new QPushButton(widget_debug);
			pick_debug_button->setObjectName("pick_debug_btn");
			pick_debug_button->setIcon(QIcon(PICK_DEBUG_BTN_ICON));
			pick_debug_button->setIconSize(icon_size);
			pick_debug_button->setToolTip(QString::fromUtf8("������ر�ѡȡ����������"));
			pick_debug_button->setCheckable(true);

			layout_debug->addWidget(label_ambient_debug);
			layout_debug->addWidget(spin_box_ambient_debug);
			layout_debug->addWidget(label_diffuse_debug);
			layout_debug->addWidget(spin_box_diffuse_debug);
			layout_debug->addWidget(label_specular_debug);
			layout_debug->addWidget(spin_box_specular_debug);
			layout_debug->addWidget(pick_debug_button);

			connect(spin_box_ambient_debug, SIGNAL(valueChanged(double)),
				openGLWidget, SLOT(setAmbient(double)));
			connect(spin_box_diffuse_debug, SIGNAL(valueChanged(double)),
				openGLWidget, SLOT(setDiffuse(double)));
			connect(spin_box_specular_debug, SIGNAL(valueChanged(double)),
				openGLWidget, SLOT(setSpecular(double)));
			connect(pick_debug_button, SIGNAL(toggled(bool)), openGLWidget, SLOT(setFuncAble(bool)));

		}
	}

	// �Ҳ��ƶ���ת���Ź��ܼ�
	QFrame* func_widget = new QFrame(this->centralWidget());
	
	
	loadStyleSheet(this, TAB_WIDGET_STYLE);

	// layout���и��ؼ�
	QVBoxLayout* layout = new QVBoxLayout(this->centralWidget());
	layout->addWidget(title_bar);
	layout->addWidget(tab_widget);
	layout->addWidget(openGLWidget);
	layout->setSpacing(0);
	layout->setMargin(0);
	this->setContentsMargins(0, 0, 0, 0);
	this->centralWidget()->setContentsMargins(0, 0, 0, 0);

	connect(save_button, SIGNAL(clicked()), this, SLOT(openSaveFileDialog()));
	connect(save_as_button, SIGNAL(clicked()), this, SLOT(openSaveFileDialog()));
	connect(render_button, SIGNAL(clicked()), this, SLOT(callRender()));
	connect(this, SIGNAL(saveModels(QString)), openGLWidget, SLOT(saveModels(QString)));
}

void Builder::loadStyleSheet(QWidget* widget, const QString& styleSheetFile)
{
	QFile file(styleSheetFile);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = widget->styleSheet();
		styleSheet += QString::fromStdString(file.readAll().toStdString());//��ȡ��ʽ���ļ�
		QString additon_style = tr("QTabWidget {min-height: ") + QString::number(120 * screen_relative_y_pix) +
			tr("px;max-height: ") + QString::number(120 * screen_relative_y_pix) + tr("px;}");
		additon_style += tr("QPushButton {height: ") + QString::number(60 * screen_relative_y_pix) +
			tr("px;width: ") + QString::number(62 * screen_relative_x_pix) + tr("px;}");
		additon_style += tr("QTabBar::tab {width:") + QString::number(85 * screen_relative_x_pix) +
			tr("px;height: ") + QString::number(60 * screen_relative_y_pix) + tr("px;}");
		additon_style += tr("QTabBar::tab {font-size: ") + QString::number(16 * screen_relative_y_pix) + tr("px;}");
		additon_style += tr("QTabBar::pane {font-size: ") + QString::number(16 * screen_relative_y_pix) + tr("px;}");
		additon_style += tr("QPushButton {font-size: ") + QString::number(16 * screen_relative_y_pix) + tr("px;}");
		widget->setStyleSheet(styleSheet + additon_style);//���ļ����ݴ���
		file.close();
	}
	else
	{
		QMessageBox::information(widget, "tip", "cannot find qss file");
	}
}

bool Builder::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	// �������������������ؿ��
	int padding = 5;
	// ����windows native�ź�
	MSG* ms_message = static_cast<MSG*>(message);
	switch (ms_message->message)
	{
	case WM_NCHITTEST:
		int xPos = GET_X_LPARAM(ms_message->lParam) - this->frameGeometry().x();
		int yPos = GET_Y_LPARAM(ms_message->lParam) - this->frameGeometry().y();
		if (xPos < padding && yPos < padding)
		{
			*result = HTTOPLEFT;
			return true;
		}
		if (yPos < padding && xPos >(this->width() - padding)) {
			*result = HTTOPRIGHT;
			return true;
		}
		if (yPos > (this->height() - padding) && xPos < padding)
		{
			*result = HTBOTTOMLEFT;
			return true;
		}
		if (yPos > (this->height() - padding) && xPos > (this->width() - padding))
		{
			*result = HTBOTTOMRIGHT;
			return true;
		}
		if (xPos < padding)
		{
			*result = HTLEFT;
			return true;
		}
		if (xPos > (this->width() - padding))
		{
			*result = HTRIGHT;
			return true;
		}
		if (yPos < padding)
		{
			*result = HTTOP;
			return true;
		}
		if (yPos > (this->height() - padding))
		{
			*result = HTBOTTOM;
			return true;
		}
		return false;
	}
	return false;
}

bool Builder::eventFilter(QObject* watched, QEvent* event)
{
	if (watched->objectName() == "color_alpha_slider" || watched->objectName() == "reflectivity_slider")
	{
		QSlider* slider = qobject_cast<QSlider*>(watched);
		if (event->type() == QEvent::MouseButtonPress)           //�ж�����
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton) //�ж����
			{
				int dur = slider->maximum() - slider->minimum();
				int pos = slider->minimum() + dur * ((double)mouseEvent->x() / slider->width());
				if (pos < slider->maximum() * 0.025)
				{
					slider->setValue(slider->minimum());
				} else if (pos > slider->maximum() * 0.975)
				{
					slider->setValue(slider->maximum());
				} else if (pos != slider->sliderPosition())
				{
					slider->setValue(pos);
				}
			}
		}
	}
	return QObject::eventFilter(watched, event);
}

std::string Builder::ExeCmd(std::string pszCmd)
{
	// ���������ܵ�
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return string(" ");
	}

	// ���������н���������Ϣ(�����ط�ʽ���������λ�������hWrite
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;

	// ����������
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, (LPWSTR)pszCmd.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return string("Cannot create process");
	}

	// �����ر�hWrite
	CloseHandle(hWrite);

	// ��ȡ�����з���ֵ
	std::string strRetTmp;
	char buff[1024] = { 0 };
	DWORD dwRead = 0;
	strRetTmp = buff;
	while (ReadFile(hRead, buff, 1024, &dwRead, NULL))
	{
		strRetTmp += buff;
	}
	CloseHandle(hRead);

	LPCSTR pszSrc = strRetTmp.c_str();
	int nLen = MultiByteToWideChar(936, 0, buff, -1, NULL, 0);
	if (nLen == 0)
		return std::string("");

	char* pwszDst = new char[nLen];
	if (!pwszDst)
		return std::string("");

	MultiByteToWideChar(936, 0, pszSrc, -1, (LPWSTR)pwszDst, nLen);
	std::string strRet(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return strRet;
}

void Builder::openModelFileDialog()
{
	QString path = QFileDialog::getOpenFileName(this,
		QString::fromUtf8("��ģ���ļ�"), "",
		QString::fromUtf8("�����ļ�(*.json)"));
	emit loadModelPath(path);
}

void Builder::openColorDialog()
{
	QColorDialog dialog(Qt::red, this);
	dialog.setOption(QColorDialog::ShowAlphaChannel);
	dialog.exec(); 
	QColor color = dialog.currentColor();
	emit colorSelected(color);
	qDebug() << "color" << color;
}

void Builder::openSaveFileDialog()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (btn->objectName() == "save_btn")
	{
		if (this->file_path != "")
		{
			emit saveModels(this->file_path);
			return;
		}
	}
	this->file_path = QFileDialog::getSaveFileName(this, QString::fromUtf8("����"), 
		"", "�����ļ�(*.json)");
	emit saveModels(this->file_path);
}

void Builder::callRender()
{
	save_button->click();
	string cmd = "E:/Raytracing_CUDA.exe " + file_path.toStdString();
	string rts = ExeCmd(cmd);
	QString str(rts.c_str());
	if (str.contains("Render done: ", Qt::CaseSensitive))
	{
		qDebug() << "Render done.";
		QStringList str_list = str.split(": ", QString::SkipEmptyParts);
		QString image_path = str_list[1];
		qDebug() << "image_path: " << image_path;
		QDialog dialog;
		dialog.setFixedSize(1080, 800);
		QLabel label(&dialog);
		label.setPixmap(QPixmap(image_path));
		dialog.exec();
	}
}


