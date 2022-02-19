#include "MyOpenGLWidget.h"
#include <QtGui/QKeyEvent>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	// 设置OpenGL窗口为强聚焦，意为鼠标移入则追踪鼠标位置
	this->setFocusPolicy(Qt::StrongFocus);
	// 设置窗口为相对父窗口最大化
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	// 设置OpenGL版本信息
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 4);
	setFormat(format);
	// 设置鼠标为默认鼠标
	setCursor(Qt::ArrowCursor);
}

MyOpenGLWidget::~MyOpenGLWidget()
{
	delete program;
	delete skybox;
	delete floor;
	delete program_floor;
	delete program_skybox;
	delete program_pick;
	delete program_single_color;
	delete m_picking_texture;
	// delete my_camera;
}

void MyOpenGLWidget::insertInsideModel()
{
	// 捕捉当前OpenGL上下文
	this->makeCurrent();
	QFileInfo file_info;
	// 根据发出信号的按钮的对象名确定载入何种内建模型
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());
	switch (_hash_(pButton->objectName().toStdString().c_str()))
	{
	case "cube_btn"_hash:
		{
		loadModel(MODEL_CUBE);
		}
		break;
	case "cylinder_btn"_hash:
		{
		loadModel(MODEL_CYLINDER);
		}
		break;
	case "pyramid_btn"_hash:
		{
		loadModel(MODEL_PYRAMID);
		}
		break;
	case "cone_btn"_hash:
	{
		loadModel(MODEL_CONE);
	}
	break;
	case "ball_btn"_hash:
	{
		loadModel(MODEL_BALL);
	}
	break;
	case "hexagon_btn"_hash:
	{
		loadModel(MODEL_HEXAGON);
	}
	break;
	case "ring_btn"_hash:
	{
		loadModel(MODEL_RING);
	}
	break;
	case "tetrahedron_btn"_hash:
	{
		loadModel(MODEL_TETRAHEDRON);
	}
	break;
	case "rabbit_btn"_hash:
		{
		loadModel(MODEL_BUNNY);
		}
	break;
	default:
		break;
	}
	// Model* model = new Model(file_info.absoluteFilePath(), this->skybox);
	// model->id = ourModel.count();
	// ourModel.insert(model->id, model);
}

void MyOpenGLWidget::loadModel(QString path)
{
	if (path == "") return;
	this->makeCurrent();

	FileReader reader(path.toStdString());
	vector<Ivy_Model> tmp;
	reader.getModelList(tmp);
	for (Ivy_Model model : tmp)
	{
		Model* newModel = new Model(path, this->skybox);
		for (int indice : model.getindice())
		{
			newModel->_indices.append(indice);
		}
		vector<Ivy233::vec3<float>> normals = model.getnormal();
		vector<Ivy233::vec3<float>> vertices = model.getvertices();
		for (int i = 0; i < normals.size(); i++)
		{
			Vertex vertex;
			vertex.Position = { vertices[i].x, vertices[i].y, vertices[i].z };
			vertex.Normal = { normals[i].x, normals[i].y, normals[i].z };
			newModel->vertices.push_back(vertex);
		}
		int materal_id = model.getmat();
		Material* mat = reader.getMaterialByIndex(materal_id);
		float color_x, color_y, color_z;
		mat->getColor(color_x, color_y, color_z);
		newModel->color = QVector3D(color_x, color_y, color_z);
		newModel->alpha = mat->getalpha();
		qDebug() << newModel->color;
		newModel->id = ourModel.count();
		newModel->reflectivity = mat->getreflect();
		ourModel.insert(newModel->id, newModel);
		newModel->setUpModel();
	}
	qDebug() << "load success";
	// Model* model = new Model(path, this->skybox);
	// model->id = ourModel.count();
	// ourModel.insert(model->id, model);
}


void MyOpenGLWidget::setAmbient(double value)
{
	this->ambient_strength = value;
}

void MyOpenGLWidget::setDiffuse(double value)
{
	this->diffuse_strength = value;
}

void MyOpenGLWidget::setSpecular(double value)
{
	this->specular_strength = value;
}

void MyOpenGLWidget::setFuncAble(bool value)
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	switch (_hash_(btn->objectName().toStdString().c_str()))
	{
	case "select_obj_btn"_hash:
		{
			// 若设置为对象不可选中则清空所有对象的选中状态
			if (value == false)
			{
				this->object_select_able = false;
				for (Model* model : ourModel.values())
				{
					model->is_selected = false;
				}
			}
			else
			{
				this->object_select_able = true;
			}
		}
	break;
	case "select_point_btn"_hash:
		{
			if (value == false)
			{
				this->point_select_able = false;
				for (Model* model : ourModel.values())
				{
					model->points.clearPoints();
				}
			} else
			{
				this->point_select_able = true;
			}
		}
		break;
	case "wire_frame_btn"_hash:
		{
			this->wire_frame_able = value;
		}
		break;
	case "pick_debug_btn"_hash:
		{
			this->pick_debug_able = value;
		}
		break;
	default:
		break;
	}
}

void MyOpenGLWidget::colorSelected(QColor color)
{
	for (Model* model : ourModel.values())
	{
		if (model->is_selected)
		{
			model->color = QVector3D(color.red(), color.green(), color.blue()).normalized();
		}
	}
}

void MyOpenGLWidget::alphaChanged(int value)
{
	for (Model *model : ourModel.values())
	{
		if (model->is_selected)
		{
			model->alpha = value / 10000.0f;
		}
	}
}

void MyOpenGLWidget::reflectivityChanged(int value)
{
	for (Model* model : ourModel.values())
	{
		if (model->is_selected)
		{
			model->reflectivity = value / 10000.0f;
		}
	}
}

void MyOpenGLWidget::saveModels(QString path)
{
	// 保存所有模型到一个场景
	qDebug() << "save scene:" << path;
	FileReader reader;

	vector<Material*> materials;
	QMapIterator<int, Model*> it(ourModel);
	while (it.hasNext())
	{
		it.next();
		Ivy_Model save_model;
		save_model.setfiledir((QString("obj_") + QString::number(it.value()->id)).toStdString());
		// save_model.setindice(it.value()->_indices.toStdVector());
		vector<Ivy233::vec3<float>> vertices;
		vector<int> indices;
		for (Vertex vertex : it.value()->vertices)
		{
			vertex.Position = it.value()->model * vertex.Position;
			vertices.push_back({ vertex.Position.x(), vertex.Position.y(), vertex.Position.z() });
		}
		for (unsigned int i : it.value()->_indices)
		{
			indices.push_back(i);
		}
		Material* mat = new Material;
		float color_x, color_y, color_z;
		color_x = it.value()->color.x();
		color_y = it.value()->color.y();
		color_z = it.value()->color.z();
		mat->setColor(color_x, color_y, color_z);
		mat->setalpha(it.value()->alpha);
		mat->setreflect(it.value()->reflectivity);
		materials.push_back(mat);
		save_model.setindice(indices);
		save_model.setmaterial(it.value()->id);
		save_model.setvertice(vertices);
		reader.setModel(save_model);
	}
	reader.setModelMaterial(materials);
	reader.setCameraLookAt(this->m_camera.viewCenter().x(), this->m_camera.viewCenter().y(), this->m_camera.viewCenter().z());
	reader.setCameraLookFrom(this->m_camera.position().x(), this->m_camera.position().y(), this->m_camera.position().z());
	reader.setCameraFocusDist(QVector3D(this->m_camera.position() - this->m_camera.viewCenter()).length());
	reader.setRenderParam(200, 100, 10);
	reader.setCameraVfov(35);
	reader.setCameraAspect(2);
	reader.setCameraAperture(0.1);
	reader.setCameraVup(0, 0, 1);
	reader.writeFile(path.toStdString());
}


void MyOpenGLWidget::initializeGL()
{
	// 为当前环境初始化OpenGL函数
	initializeOpenGLFunctions();
	printContextInformation();
	glClearColor(196.0 / 255.0, 196.0 / 255.0, 196.0 / 255.0, 1.0f);
	// glPolygonMode(GL_FRONT, GL_LINE);
	// 深度测试
	glEnable(GL_DEPTH_TEST);
	// 模板测试
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 启用面剔除
	glEnable(GL_CULL_FACE);
	// 面顺序，CW为顺时针为正向面
	glFrontFace(GL_CCW);
	// 启用混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	initProgram();

	this->skybox = new SkyBox;
	this->floor = new MyFloor;
	this->point = new MyPoint;
	this->m_picking_texture = new PickingTexture;
	this->m_picking_texture->Init(this->width(), this->height());
	initCamera();
	this->arrow = new MyArrow(m_camera.position());
	connect(&m_camera, SIGNAL(cameraPosChanged(QVector3D)), arrow, SLOT(updateCameraPos(QVector3D)));
}

void MyOpenGLWidget::paintGL()
{
	const int w = width();
	const int h = height();
	glViewport(0, 0, w, h);
	
	glDisable(GL_BLEND);
	// 拾取绘制阶段，使用颜色拾取原理，给每个顶点设置一个独特的颜色值，输出到自定义帧缓冲中
	if (!ourModel.isEmpty())
	{
		m_picking_texture->EnableWriting();
		// 注意：此处清除缓冲区必须在绑定自定义缓冲之后，以便能够清除自定义缓冲
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (const Model *model : ourModel.values())
		{
			program_pick->bind();
			program_pick->setUniformValue("view", m_camera.viewMatrix());
			program_pick->setUniformValue("projection", m_camera.projectionMatrix());
			program_pick->setUniformValue("model", model->model);
			program_pick->setUniformValue("debug", false);
			program_pick->setUniformValue("ViewportMatrix", m_camera.viewPortMatrix());
			program_pick->setUniformValue("gObjectIndex", model->id);
			model->Draw(program_pick, true);
			program_pick->release();
		}
		if (!this->arrow->selected_models.empty())
		{
			program_arrow->bind();
			program_arrow->setUniformValue("view", m_camera.viewMatrix());
			program_arrow->setUniformValue("projection", m_camera.projectionMatrix());
			program_arrow->setUniformValue("viewPort", m_camera.viewPortMatrix());
			program_arrow->setUniformValue("pick", true);
			arrow->Draw(program_arrow, true);
			program_arrow->release();
		}
		
		m_picking_texture->DisableWriting();
	}
	glEnable(GL_BLEND);

	// 清除颜色、深度和模板缓存
	glClearColor(0.76078f, 0.76078f, 0.76078f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// glDisable(GL_BLEND);

	// 绘制天空盒
	glStencilMask(0x00);
	program_skybox->bind();
	program_skybox->setUniformValue("view", QMatrix4x4(m_camera.viewMatrix().normalMatrix()));
	program_skybox->setUniformValue("projection", m_camera.projectionMatrix());
	skybox->Draw(program_skybox);
	program_skybox->release();

	// 如果摄像机在地板上方，则先绘制地板，以使物体透明后可以看到地板
	// 为什么这样做：采取的透明绘制方式有顺序限制，即靠前物体若要能看到后方被遮挡物体，则需要更晚绘制
	if (m_camera.position().y() >= 0)
	{
		// 绘制地板
	// 禁止模板缓冲写入
		glStencilMask(0x00);
		glDisable(GL_CULL_FACE);
		// glDepthFunc(GL_NOTEQUAL);
		program_floor->bind();
		program_floor->setUniformValue("view", m_camera.viewMatrix());
		program_floor->setUniformValue("projection", m_camera.projectionMatrix());
		this->floor->Draw(program_floor);
		program_floor->release();
		glEnable(GL_CULL_FACE);
		// glDepthFunc(GL_LESS);
		glStencilMask(0xFF);
	}

	// 若模型已加载，则绘制物体
	if (!ourModel.isEmpty()) {
		// 模板缓冲在物体绘制的区域都更新为1
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// 对每个对象按照与摄像机的距离从小到大排序，在绘制时首先绘制距离最大的对象
		QMap<float, const Model*> sortedObj;
		for(const Model *model : ourModel.values())
		{
			float distance = QVector3D(m_camera.position() - model->model * model->view_Center).length();
			if (sortedObj.contains(distance)) distance += 0.001f;
			sortedObj[distance] = model;
		}
		QMapIterator<float, const Model*> it(sortedObj);
		it.toBack();
		while (it.hasPrevious())
		{
			it.previous();
			const Model* model = it.value();
			if (pick_debug_able)
			{
				glDisable(GL_BLEND);
				program_pick->bind();
				program_pick->setUniformValue("debug", true);
				program_pick->setUniformValue("view", m_camera.viewMatrix());
				program_pick->setUniformValue("projection", m_camera.projectionMatrix());
				program_pick->setUniformValue("model", model->model);
				program_pick->setUniformValue("ViewportMatrix", m_camera.viewPortMatrix());
				program_pick->setUniformValue("gObjectIndex", model->id);
				// qDebug() << model.id;
				model->Draw(program_pick, true);
				program_pick->release();
				glEnable(GL_BLEND);
			}
			else
			{
				program->bind();
				// 绘制物体
				program->setUniformValue("view", m_camera.viewMatrix());
				program->setUniformValue("projection", m_camera.projectionMatrix());
				program->setUniformValue("model", model->model);
				program->setUniformValue("Line.width", 0.1f);
				program->setUniformValue("Line.color", 0.0f, 0.0f, 0.0f, 1.0f);
				program->setUniformValue("ViewportMatrix", m_camera.viewPortMatrix());
				program->setUniformValue("isWireFrame", wire_frame_able);
				program->setUniformValue("light.position", m_camera.position());
				program->setUniformValue("light.ambient", ambient_strength, ambient_strength, ambient_strength);
				program->setUniformValue("light.diffuse", diffuse_strength, diffuse_strength, diffuse_strength);
				program->setUniformValue("light.specular", specular_strength, specular_strength, specular_strength);
				program->setUniformValue("viewPos", m_camera.position());
				model->Draw(program);
				program->release();
			}

			if (model->points.has_points)
			{
				program_point->bind();
				program_point->setUniformValue("view", m_camera.viewMatrix());
				program_point->setUniformValue("projection", m_camera.projectionMatrix());
				program_point->setUniformValue("model", model->model);
				model->DrawPoint(program_point);
				program_point->release();
			}
		}
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// 禁止模板缓冲写入
		glStencilMask(0x00);
		// 绘制物体边框
		glDepthFunc(GL_ALWAYS);
		for (const Model* model : ourModel.values())
		{
			if (model->is_selected)
			{
				program_single_color->bind();
				program_single_color->setUniformValue("view", m_camera.viewMatrix());
				program_single_color->setUniformValue("projection", m_camera.projectionMatrix());
				program_single_color->setUniformValue("model", model->model);
				float ratio = (m_camera.position() - model->view_Center).length();
				program_single_color->setUniformValue("scale_ratio", ratio / 100.0f);
				model->Draw(program_single_color);
				program_single_color->release();
			}
		}
		glDepthFunc(GL_LESS);
		glStencilMask(0xFF);
	}

	// 绘制箭头
	// glStencilFunc(GL_ALWAYS, 1, 0xFF);
	// if (!this->arrow->selected_models.empty())
	// {
	// 	program_arrow->bind();
	// 	program_arrow->setUniformValue("view", m_camera.viewMatrix());
	// 	program_arrow->setUniformValue("projection", m_camera.projectionMatrix());
	// 	program_arrow->setUniformValue("viewPort", m_camera.viewPortMatrix());
	// 	program_arrow->setUniformValue("pick", false);
	// 	arrow->Draw(program_arrow);
	// 	program_arrow->release();
	// }
	
	if (m_camera.position().y() < 0)
	{
		// 绘制地板
	// 禁止模板缓冲写入
		glStencilMask(0x00);
		glDisable(GL_CULL_FACE);
		// glDepthFunc(GL_NOTEQUAL);
		program_floor->bind();
		program_floor->setUniformValue("view", m_camera.viewMatrix());
		program_floor->setUniformValue("projection", m_camera.projectionMatrix());
		this->floor->Draw(program_floor);
		program_floor->release();
		glEnable(GL_CULL_FACE);
		// glDepthFunc(GL_LESS);
		glStencilMask(0xFF);
	}
	
	const qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	if ((currentTime % 10) == 0)
	{
		// qDebug() << "FPS:" << static_cast<int>(1000 / deltaTime);
	}
	update();
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	// 如果选中框有物体被选中则移动
	if (this->arrow->is_selected)
	{
		if (this->key_func == 1)
		{
			switch (event->key())
			{
			case Qt::Key_W:
				this->arrow->moveArrow(Back);
				break;
			case Qt::Key_A:
				this->arrow->moveArrow(Left);
				break;
			case Qt::Key_D:
				this->arrow->moveArrow(Right);
				break;
			case Qt::Key_S:
				this->arrow->moveArrow(Front);
				break;
			case Qt::Key_Q:
				this->arrow->moveArrow(Up);
				break;
			case Qt::Key_E:
				this->arrow->moveArrow(Down);
				break;
			case Qt::Key_Delete:
				for (Model* model : this->arrow->selected_models.values())
				{
					this->arrow->objUnSelected(model);
					this->ourModel.remove(model->id);
				}
				break;
			case Qt::Key_1:
				this->key_func = 1;
				break;
			case Qt::Key_2:
				this->key_func = 2;
				break;
			case Qt::Key_3:
				this->key_func = 3;
				break;
			default:
				break;
			}
		} else if (this->key_func == 2)
		{
			switch (event->key())
			{
			case Qt::Key_W:
				this->arrow->modelsRotate(X_Reverse);
				break;
			case Qt::Key_A:
				this->arrow->modelsRotate(Z_Forward);
				break;
			case Qt::Key_D:
				this->arrow->modelsRotate(Z_Reverse);
				break;
			case Qt::Key_S:
				this->arrow->modelsRotate(X_Forward);
				break;
			case Qt::Key_Q:
				this->arrow->modelsRotate(Y_Forward);
				break;
			case Qt::Key_E:
				this->arrow->modelsRotate(Y_Reverse);
				break;
			case Qt::Key_Delete:
				for (Model* model : this->arrow->selected_models.values())
				{
					this->arrow->objUnSelected(model);
					this->ourModel.remove(model->id);
				}
				break;
			case Qt::Key_1:
				this->key_func = 1;
				break;
			case Qt::Key_2:
				this->key_func = 2;
				break;
			case Qt::Key_3:
				this->key_func = 3;
				break;
			default:
				break;
			}
		} else
		{
			switch (event->key())
			{
			case Qt::Key_W:
				this->arrow->modelsScale(Z_Negative);
				break;
			case Qt::Key_A:
				this->arrow->modelsScale(X_Negative);
				break;
			case Qt::Key_D:
				this->arrow->modelsScale(X_Positive);
				break;
			case Qt::Key_S:
				this->arrow->modelsScale(Z_Positive);
				break;
			case Qt::Key_Q:
				this->arrow->modelsScale(Y_Positive);
				break;
			case Qt::Key_E:
				this->arrow->modelsScale(Y_Negative);
				break;
			case Qt::Key_Delete:
				for (Model* model : this->arrow->selected_models.values())
				{
					this->arrow->objUnSelected(model);
					this->ourModel.remove(model->id);
				}
				break;
			case Qt::Key_1:
				this->key_func = 1;
				break;
			case Qt::Key_2:
				this->key_func = 2;
				break;
			case Qt::Key_3:
				this->key_func = 3;
				break;
			default:
				break;
			}
		}
	}
	update();
	QOpenGLWidget::keyPressEvent(event);
}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event)
{
	// my_camera->ProcessMouseScroll(event->delta());
	this->m_camera.zoom(event->delta());
	update();
	QOpenGLWidget::wheelEvent(event);
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		QPointF tmp = lastMousePos;
		lastMousePos = event->localPos();
		QPointF delta = lastMousePos - tmp;
		this->m_camera.panAboutViewCenter(-delta.x(), { 0.0f, 1.0f, 0.0f });
		this->m_camera.tiltAboutViewCenter(delta.y());
		// 如果是移动物体模式
		// if (this->move_mode)
		// {
			// 传入摄像机屏幕平面的法向量，用于计算得出真实的物体移动情况
			// this->arrow->moveArrow(delta, m_camera.viewVector(), Up);
		// } else
		// {
			// this->m_camera.panAboutViewCenter(-delta.x(), { 0.0f, 1.0f, 0.0f });
			// // this->m_camera.panAboutViewCenter(-delta.x());
			// this->m_camera.tiltAboutViewCenter(delta.y());
		// }
	} else if (event->buttons() == Qt::RightButton)
	{
		QPointF tmp = lastMousePos;
		lastMousePos = event->localPos();
		QVector3D delta;
		delta.setX(tmp.x() - lastMousePos.x());
		delta.setY(lastMousePos.y() - tmp.y());
		this->m_camera.translate(delta);
	}
	QOpenGLWidget::mouseMoveEvent(event);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	this->makeCurrent();
	if (event->buttons() == Qt::LeftButton)
	{
		this->lastMousePos = event->localPos();
		this->move_mode = false;

		PickingTexture::PixelInfo pixel = m_picking_texture->ReadPixel(event->x(), height() - event->y());
		qDebug() << "击中对象：(普通模型ID由20起)";
		qDebug() << "objectID：" << pixel.ObjectID;
		qDebug() << "meshID：" << pixel.MeshID;
		qDebug() << "primID：" << pixel.PrimID;
		qDebug() << "vertID：" << pixel.vertID;
		if (pixel.ObjectID != 0)
		{
			// 如果点击到的是移动箭头
			if (int(pixel.ObjectID) == 1)
			{
				QCursor cursor;
				cursor.setShape(Qt::OpenHandCursor);
				setCursor(cursor);
				// 此处MeshID指代箭头ID，箭头ID的具体含义见箭头类
				this->move_mode = true;
				this->arrow->setArrowSelect(int(pixel.MeshID));
			}
			if (ourModel.contains(int(pixel.ObjectID - 20.0f)))
			{
				Model* model = ourModel.value(int(pixel.ObjectID - 20.0f));
				if (this->object_select_able)
				{
					if (model->is_selected)
					{
						model->is_selected = false;
						this->arrow->objUnSelected(model);
					}
					else
					{
						model->is_selected = true;
						this->arrow->objSelected(model);
					}
				}
				if (this->point_select_able && (int(pixel.vertID + 0.5f) != 0))
				{
					model->judgeSelectedPoint(pixel.MeshID, pixel.PrimID, pixel.vertID);
				}
			}
		}
		qDebug() << "pick end";
	} else if (event->buttons() == Qt::RightButton)
	{
		// QCursor cursor;
		// cursor.setShape(Qt::cursor)
		this->lastMousePos = event->localPos();
	}
	QOpenGLWidget::mousePressEvent(event);
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QCursor cursor;
	cursor.setShape(Qt::ArrowCursor);
	setCursor(cursor);
}

void MyOpenGLWidget::initProgram()
{
	// 创建顶点着色器
	QOpenGLShader *v_shader_model = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_model->compileSourceFile(SHADER_MODEL_VERT);
	// 创建几何着色器
	QOpenGLShader* g_shader_model = new QOpenGLShader(QOpenGLShader::Geometry, this);
	g_shader_model->compileSourceFile(SHADER_MODEL_GEOM);
	// 创建片段着色器
	QOpenGLShader* f_shader_model = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_model->compileSourceFile(SHADER_MODEL_FRAG);
	// 边框顶点着色器
	QOpenGLShader* v_shader_single_color = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_single_color->compileSourceFile(SHADER_STENCIL_SINGLE_COLOR_VERT);
	// 创建边框片段着色器
	QOpenGLShader* f_shader_single_color = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_single_color->compileSourceFile(SHADER_STENCIL_SINGLE_COLOR_FRAG);
	// 天空盒顶点着色器
	QOpenGLShader* v_shader_skybox = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_skybox->compileSourceFile(SHADER_SKYBOX_VERT);
	// 天空盒片段着色器
	QOpenGLShader* f_shader_skybox = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_skybox->compileSourceFile(SHADER_SKYBOX_FRAG);
	// 地板顶点着色器
	QOpenGLShader* v_shader_floor = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_floor->compileSourceFile(SHADER_FLOOR_VERT);
	// 地板片段着色器
	QOpenGLShader* f_shader_floor = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_floor->compileSourceFile(SHADER_FLOOR_FRAG);
	// 选取模式顶点着色器
	QOpenGLShader* v_shader_pick = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_pick->compileSourceFile(SHADER_PICK_VERT);
	// 选取模式几何着色器
	QOpenGLShader* g_shader_pick = new QOpenGLShader(QOpenGLShader::Geometry, this);
	g_shader_pick->compileSourceFile(SHADER_PICK_GEOM);
	// 选取模式片段着色器
	QOpenGLShader* f_shader_pick = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_pick->compileSourceFile(SHADER_PICK_FRAG);
	// 点顶点着色器
	QOpenGLShader* v_shader_point = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_point->compileSourceFile(SHADER_POINT_VERT);
	// 点片段着色器
	QOpenGLShader* f_shader_point = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_point->compileSourceFile(SHADER_POINT_FRAG);
	// 箭头顶点着色器
	QOpenGLShader* v_shader_arrow = new QOpenGLShader(QOpenGLShader::Vertex, this);
	v_shader_arrow->compileSourceFile(SHADER_ARROW_VERT);
	// 箭头几何着色器
	QOpenGLShader* g_shader_arrow = new QOpenGLShader(QOpenGLShader::Geometry, this);
	g_shader_arrow->compileSourceFile(SHADER_ARROW_GEOM);
	// 箭头片段着色器
	QOpenGLShader* f_shader_arrow = new QOpenGLShader(QOpenGLShader::Fragment, this);
	f_shader_arrow->compileSourceFile(SHADER_ARROW_FRAG);

	// 将各着色器与渲染程序链接
	program = new QOpenGLShaderProgram;
	program->addShader(v_shader_model);
	program->addShader(g_shader_model);
	program->addShader(f_shader_model);
	program->link();

	program_single_color = new QOpenGLShaderProgram;
	program_single_color->addShader(v_shader_single_color);
	program_single_color->addShader(f_shader_single_color);
	program_single_color->link();

	program_skybox = new QOpenGLShaderProgram;
	program_skybox->addShader(v_shader_skybox);
	program_skybox->addShader(f_shader_skybox);
	program_skybox->link();

	program_floor = new QOpenGLShaderProgram;
	program_floor->addShader(v_shader_floor);
	program_floor->addShader(f_shader_floor);
	program_floor->link();

	program_pick = new QOpenGLShaderProgram;
	program_pick->addShader(v_shader_pick);
	program_pick->addShader(g_shader_pick);
	program_pick->addShader(f_shader_pick);
	program_pick->link();

	program_point = new QOpenGLShaderProgram;
	program_point->addShader(v_shader_point);
	program_point->addShader(f_shader_point);
	program_point->link();

	program_arrow = new QOpenGLShaderProgram;
	program_arrow->addShader(v_shader_arrow);
	program_arrow->addShader(g_shader_arrow);
	program_arrow->addShader(f_shader_arrow);
	program_arrow->link();
}

void MyOpenGLWidget::initCamera()
{
	// 设置摄像机位置初值
	m_camera.setPosition({ 0.0f, 0.0f, 250.0f });
	// 将摄像机绕聚焦中心朝上方旋转，使得视角朝向斜下方
	m_camera.tiltAboutViewCenter(50.0f);
	// 设置视锥为45度
	this->m_camera.setPerspectiveProjection(45.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 10240.0f);
	this->m_camera.setViewPort(0.0f, 0.0f, this->width(), this->height());
	// 随时将摄像机位置与地板类中的摄像机位置进行同步，进而可动态调整地板大小
	connect(&m_camera, SIGNAL(cameraPosChanged(QVector3D)), floor, SLOT(cameraPosChanged(QVector3D)));
}

void MyOpenGLWidget::printContextInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	qDebug() << "Maximum nr of vertex attributes supported: " << nrAttributes;

	// Get Version Information
	glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	// Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch (format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	// qPrintable() will print our QString w/o quotes around it.
	qDebug() << qPrintable(glType) << qPrintable(glVersion)
		<< "(" << qPrintable(glProfile) << ")";

}
