#include "MyArrow.h"

MyArrow::MyArrow()
{
}

MyArrow::MyArrow(QVector3D cameraPos)
{
	this->_vertices.append({ 
		{-11.601017, 19.999222, -0.000012},
		{11.566902, 19.999222, -0.000012},
		{-0.015900, 43.165977, -0.000534},
		{-11.601017, 19.999222, -0.001012},
		{11.566902, 19.999222, -0.000012},
		{-11.601017, 19.999222, -0.000012},
		{-11.601017, 19.999222, -0.000012},
		{-0.015900, 43.165977, -0.000534},
		{-11.601017, 19.999222, -0.001012},
		{11.566902, 19.999222, -0.000012},
		{11.566902, 19.999222, -0.001012},
		{-0.015900, 43.165977, -0.000534},
		{11.566902, 19.999222, -0.001012},
		{-11.601017, 19.999222, -0.001012},
		{-0.015900, 43.165977, -0.000534},
		{11.566902, 19.999222, -0.001012},
		{11.566902, 19.999222, -0.000012},
		{-11.601017, 19.999222, -0.001012},
		{-5.455828, 19.998878, -0.000327},
		{5.489600, 19.998878, -0.000327},
		{-5.455828, 19.998878, -0.000799},
		{5.489600, -19.999050, -0.000726},
		{-5.455828, -19.999050, -0.000254},
		{-5.455828, -19.999050, -0.000726},
		{-5.455828, -19.999050, -0.000254},
		{5.489600, -19.999050, -0.000726},
		{5.489600, -19.999050, -0.000254},
		{5.489600, 19.998878, -0.000799},
		{-5.455828, 19.998878, -0.000799},
		{5.489600, 19.998878, -0.000327},
		{-5.455828, -19.999050, -0.000254},
		{5.489600, -19.999050, -0.000254},
		{5.489600, 19.998878, -0.000327},
		{-5.455828, -19.999050, -0.000254},
		{-5.455828, 19.998878, -0.000799},
		{-5.455828, -19.999050, -0.000726},
		{5.489600, 19.998878, -0.000799},
		{5.489600, 19.998878, -0.000327},
		{5.489600, -19.999050, -0.000254},
		{-5.455828, -19.999050, -0.000726},
		{-5.455828, 19.998878, -0.000799},
		{5.489600, 19.998878, -0.000799},
		{-5.455828, -19.999050, -0.000254},
		{-5.455828, 19.998878, -0.000327},
		{-5.455828, 19.998878, -0.000799},
		{5.489600, -19.999050, -0.000254},
		{5.489600, -19.999050, -0.000726},
		{5.489600, 19.998878, -0.000799},
		{5.489600, 19.998878, -0.000327},
		{-5.455828, 19.998878, -0.000327},
		{-5.455828, -19.999050, -0.000254},
		{5.489600, 19.998878, -0.000799},
		{5.489600, -19.999050, -0.000726},
		{-5.455828, -19.999050, -0.000726},
		{11.601011, -19.999052, 0.001005},
		{-11.567013, -19.999052, 0.001005},
		{0.015854, -43.165977, 0.000604},
		{11.601011, -19.999052, 0.000005},
		{-11.567013, -19.999052, 0.001005},
		{11.601011, -19.999052, 0.001005},
		{11.601011, -19.999052, 0.001005},
		{0.015854, -43.165977, 0.000604},
		{11.601011, -19.999052, 0.000005},
		{-11.567013, -19.999052, 0.001005},
		{-11.567013, -19.999052, 0.000005},
		{0.015854, -43.165977, 0.000604},
		{-11.567013, -19.999052, 0.000005},
		{11.601011, -19.999052, 0.000005},
		{0.015854, -43.165977, 0.000604},
		{-11.567013, -19.999052, 0.000005},
		{-11.567013, -19.999052, 0.001005},
		{11.601011, -19.999052, 0.000005}, });
	for (int i = 0; i < this->_vertices.size(); ++i)
	{
		this->_colors.append({ 0.0f, 0.6274f, 0.9098f });
		this->selected_move_arrow_colors.append({ 0.5019f,1.0f,0.2509f });
	}
	this->move_model.setToIdentity();

	const float max = FLT_MAX / 2;
	const float min = -max;
	this->AABB_Box[0] = { min, min, min };
	this->AABB_Box[1] = { max, max, max };
	this->select_arrow_id = -1;
	updateCameraPos(cameraPos);
	setUpModel();
}

MyArrow::~MyArrow()
{
	this->_VAO->destroy();
	this->_VBO->destroy();
	this->selected_VAO->destroy();
	this->selected_VBO->destroy();
}

void MyArrow::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	program->bind();
	QMatrix4x4 tmp;
	f->glDepthFunc(GL_ALWAYS);
	// 绘制x轴箭头
	{
		tmp = this->move_model;
		// 若摄像机相对物体在左边，则将X轴箭头显示在左边，在右边的情况相反
		if (this->arrow_to_camera.x() <= 0)
		{
			tmp.translate(this->AABB_Box[1].x() - this->arrow_distance * distance,
				(this->AABB_Box[0].y() + this->AABB_Box[1].y()) / 2.0f,
				(this->AABB_Box[0].z() + this->AABB_Box[1].z()) / 2.0f);
		} else
		{
			tmp.translate(this->AABB_Box[0].x() + this->arrow_distance * distance,
				(this->AABB_Box[0].y() + this->AABB_Box[1].y()) / 2.0f,
				(this->AABB_Box[0].z() + this->AABB_Box[1].z()) / 2.0f);
		}
		tmp.scale(distance / 1000.0f);
		// 根据摄像机的相对位置调整箭头自身的转向，使得箭头相对摄像机展示宽面
		bool up_or_down = (qAbs(this->arrow_to_camera.y()) > qAbs(this->arrow_to_camera.z())) ? true : false;
		if (up_or_down)
		{
			// 注意：箭头原方向是面朝Z轴
			tmp.rotate(-90.0f, 0, 0, 1);
			tmp.rotate(90, 0, 1, 0);
		} else
		{
			tmp.rotate(-90.0f, 0, 0, 1);
		}
		// 如果该方向箭头被选中，则绑定选中样式的箭头，解绑与之对应
		if (select_arrow_id == 0 || select_arrow_id == 1)
		{
			selected_VAO->bind();
		} else
		{
			_VAO->bind();
		}
		program->setUniformValue("model", tmp);
		if (pick_mode == true)
		{
			program->setUniformValue("gObjectIndex", 1);
			program->setUniformValue("arrowID", 0);
		}
		f->glDrawArrays(GL_TRIANGLES, 0, this->_vertices.size());
		if (select_arrow_id == 0 || select_arrow_id == 1)
		{
			selected_VAO->release();
		}
		else
		{
			_VAO->release();
		}
	}
	// 绘制y轴箭头
	{
		tmp = this->move_model;
		if (this->arrow_to_camera.y() >= 0)
		{
			tmp.translate((this->AABB_Box[0].x() + this->AABB_Box[1].x()) / 2.0f,
				this->AABB_Box[0].y() + this->arrow_distance * distance,
				(this->AABB_Box[0].z() + this->AABB_Box[1].z()) / 2.0f);
		} else
		{
			tmp.translate((this->AABB_Box[0].x() + this->AABB_Box[1].x()) / 2.0f,
				this->AABB_Box[1].y() - this->arrow_distance * distance,
				(this->AABB_Box[0].z() + this->AABB_Box[1].z()) / 2.0f);
		}
		tmp.scale(distance / 1000.0f);
		bool left_or_right = (qAbs(this->arrow_to_camera.x()) > qAbs(this->arrow_to_camera.z())) ? true : false;
		if (left_or_right)
		{
			// 注意：箭头原方向是面朝Z轴
			tmp.rotate(90, 0, 1, 0);
		}
		if (select_arrow_id == 2 || select_arrow_id == 3)
		{
			selected_VAO->bind();
		}
		else
		{
			_VAO->bind();
		}
		program->setUniformValue("model", tmp);
		if (pick_mode == true)
		{
			program->setUniformValue("gObjectIndex", 1);
			program->setUniformValue("arrowID", 2);
		}
		f->glDrawArrays(GL_TRIANGLES, 0, this->_vertices.size());
		if (select_arrow_id == 2 || select_arrow_id == 3)
		{
			selected_VAO->release();
		}
		else
		{
			_VAO->release();
		}
	}
	// 绘制z轴箭头
	{
		tmp = this->move_model;
		if (this->arrow_to_camera.z() >= 0)
		{
			tmp.translate((this->AABB_Box[0].x() + this->AABB_Box[1].x()) / 2.0f,
				(this->AABB_Box[0].y() + this->AABB_Box[1].y()) / 2.0f,
				this->AABB_Box[0].z() + this->arrow_distance * distance);
		} else
		{
			tmp.translate((this->AABB_Box[0].x() + this->AABB_Box[1].x()) / 2.0f,
				(this->AABB_Box[0].y() + this->AABB_Box[1].y()) / 2.0f,
				this->AABB_Box[1].z() - this->arrow_distance * distance);
		}
		tmp.scale(distance / 1000.0f);
		bool front_or_back = (qAbs(this->arrow_to_camera.x()) > qAbs(this->arrow_to_camera.y())) ? true : false;
		if (front_or_back)
		{
			tmp.rotate(-90.0f, 1, 0, 0);
			tmp.rotate(90, 0, 1, 0);
		} else
		{
			tmp.rotate(-90.0f, 1, 0, 0);
		}
		if (select_arrow_id == 4 || select_arrow_id == 5)
		{
			selected_VAO->bind();
		}
		else
		{
			_VAO->bind();
		}
		program->setUniformValue("model", tmp);
		if (pick_mode == true)
		{
			program->setUniformValue("gObjectIndex", 1);
			program->setUniformValue("arrowID", 4);
		}
		f->glDrawArrays(GL_TRIANGLES, 0, this->_vertices.size());
		if (select_arrow_id == 4 || select_arrow_id == 5)
		{
			selected_VAO->release();
		}
		else
		{
			_VAO->release();
		}
	}
	f->glDepthFunc(GL_LESS);
	program->release();
}

void MyArrow::setArrowSelect(int arrowID)
{
	this->select_arrow_id = arrowID;
}

void MyArrow::moveArrow(ModelMoveState state)
{
	// 计算选中框的大小在三轴上最大的，用于按比例移动
	float x_max = AABB_Box[0].x() - AABB_Box[1].x();
	float y_max = AABB_Box[0].y() - AABB_Box[1].y();
	float max_size = qMax(x_max, y_max);
	for(Model* model : this->selected_models)
	{
		// 移动选中框内的所有模型
		switch (state)
		{
		case Up:
			model->move_model.translate(0.0f, max_size / 40.0f, 0.0f);
			break;
		case Down:
			model->move_model.translate(0.0f, -max_size / 40.0f, 0.0f);
			break;
		case Left:
			model->move_model.translate(-max_size / 40.0f, 0.0f, 0.0f);
			break;
		case Right:
			model->move_model.translate(max_size / 40.0f, 0.0f, 0.0f);
			break;
		case Front:
			model->move_model.translate(0.0f, 0.0f, max_size / 40.0f);
			break;
		case Back:
			model->move_model.translate(0.0f, 0.0f, -max_size / 40.0f);
			break;
		}
		model->model = model->move_model * model->rotate_model * model->scale_model * model->org_model;
	}
	// switch (state)
	// {
	// case Up:
	// 	this->move_model.translate(0.0f, max_size / 40.0f, 0.0f);
	// 	break;
	// case Down:
	// 	this->move_model.translate(0.0f, -max_size / 40.0f, 0.0f);
	// 	break;
	// case Left:
	// 	this->move_model.translate(-max_size / 40.0f, 0.0f, 0.0f);
	// 	break;
	// case Right:
	// 	this->move_model.translate(max_size / 40.0f, 0.0f, 0.0f);
	// 	break;
	// case Front:
	// 	this->move_model.translate(0.0f, 0.0f, max_size / 40.0f);
	// 	break;
	// case Back:
	// 	this->move_model.translate(0.0f, 0.0f, -max_size / 40.0f);
	// 	break;
	// }
}

void MyArrow::modelsRotate(ModelRotateState state)
{
	// 计算选中框的大小在三轴上最大的，用于按比例移动
	float x_max = AABB_Box[0].x() - AABB_Box[1].x();
	float y_max = AABB_Box[0].y() - AABB_Box[1].y();
	float max_size = qMax(x_max, y_max);
	for (Model* model : this->selected_models)
	{
		// 移动选中框内的所有模型
		switch (state)
		{
		case X_Forward:
			model->rotate_model.rotate(10.0f, 1, 0, 0);
			break;
		case X_Reverse:
			model->rotate_model.rotate(10.0f, -1, 0, 0);
			break;
		case Y_Forward:
			model->rotate_model.rotate(10.0f, 0, 1, 0);
			break;
		case Y_Reverse:
			model->rotate_model.rotate(10.0f, 0, -1, 0);
			break;
		case Z_Forward:
			model->rotate_model.rotate(10.0f, 0, 0, 1);
			break;
		case Z_Reverse:
			model->rotate_model.rotate(10.0f, 0, 0, -1);
			break;
		}
		model->model = model->move_model * model->rotate_model * model->scale_model * model->org_model;
	}
	// switch (state)
	// {
	// case X_Forward:
	// 	this->move_model.rotate(10.0f, 1, 0, 0);
	// 	break;
	// case X_Reverse:
	// 	this->move_model.rotate(10.0f, -1, 0, 0);
	// 	break;
	// case Y_Forward:
	// 	this->move_model.rotate(10.0f, 0, 1, 0);
	// 	break;
	// case Y_Reverse:
	// 	this->move_model.rotate(10.0f, 0, -1, 0);
	// 	break;
	// case Z_Forward:
	// 	this->move_model.rotate(10.0f, 0, 0, 1);
	// 	break;
	// case Z_Reverse:
	// 	this->move_model.rotate(10.0f, 0, 0, -1);
	// 	break;
	// }
}

void MyArrow::modelsScale(ModelScaleState state)
{
	// 计算选中框的大小在三轴上最大的，用于按比例移动
	float x_max = AABB_Box[0].x() - AABB_Box[1].x();
	float y_max = AABB_Box[0].y() - AABB_Box[1].y();
	float max_size = qMax(x_max, y_max);
	for (Model* model : this->selected_models)
	{
		// 移动选中框内的所有模型
		switch (state)
		{
		case X_Positive:
			model->scale_model.scale(1.1f, 1.0f, 1.0f);
			break;
		case X_Negative:
			model->scale_model.scale(0.9f, 1.0f, 1.0f);
			break;
		case Y_Positive:
			model->scale_model.scale(1.0f, 1.1f, 1.0f);
			break;
		case Y_Negative:
			model->scale_model.scale(1.0f, 0.9f, 1.0f);
			break;
		case Z_Positive:
			model->scale_model.scale(1.0f, 1.0f, 1.1f);
			break;
		case Z_Negative:
			model->scale_model.scale(1.0f, 1.0f, 0.9f);
			break;
		}
		model->model = model->move_model * model->rotate_model * model->scale_model * model->org_model;
	}
	// switch (state)
	// {
	// case Up:
	// 	this->move_model.translate(0.0f, max_size / 40.0f, 0.0f);
	// 	break;
	// case Down:
	// 	this->move_model.translate(0.0f, -max_size / 40.0f, 0.0f);
	// 	break;
	// case Left:
	// 	this->move_model.translate(-max_size / 40.0f, 0.0f, 0.0f);
	// 	break;
	// case Right:
	// 	this->move_model.translate(max_size / 40.0f, 0.0f, 0.0f);
	// 	break;
	// case Front:
	// 	this->move_model.translate(0.0f, 0.0f, max_size / 40.0f);
	// 	break;
	// case Back:
	// 	this->move_model.translate(0.0f, 0.0f, -max_size / 40.0f);
	// 	break;
	// }
}

void MyArrow::updateCameraPos(QVector3D pos)
{
	// 更新摄像机位置
	this->cameraPos = pos;
	// 更新摄像机到箭头的距离
	distance = (this->cameraPos - this->move_model * QVector3D(0, 0, 0)).length();
	// 更新箭头到摄像机向量
	this->arrow_to_camera = (pos - this->move_model * QVector3D(0, 0, 0)).normalized();
}

void MyArrow::objSelected(Model* model)
{
	this->move_model.setToIdentity();
	this->is_selected = true;
	this->selected_models.insert(model->id, model);
	QVector3D tmp[2];
	tmp[0] = model->model * model->AABB_Box[0];
	tmp[1] = model->model * model->AABB_Box[1];
	AABB_Box[0].setX(qMax(AABB_Box[0].x(), tmp[0].x()));
	AABB_Box[0].setY(qMax(AABB_Box[0].y(), tmp[0].y()));
	AABB_Box[0].setZ(qMax(AABB_Box[0].z(), tmp[0].z()));
	AABB_Box[1].setX(qMin(AABB_Box[1].x(), tmp[1].x()));
	AABB_Box[1].setY(qMin(AABB_Box[1].y(), tmp[1].y()));
	AABB_Box[1].setZ(qMin(AABB_Box[1].z(), tmp[1].z()));
}

void MyArrow::objUnSelected(Model* model)
{
	this->move_model.setToIdentity();
	this->selected_models.remove(model->id);
	if (this->selected_models.size() == 0) this->is_selected = false;
	const float max = FLT_MAX / 2;
	const float min = -max;
	this->AABB_Box[0] = { min, min, min };
	this->AABB_Box[1] = { max, max, max };
	for(const Model *item : this->selected_models.values())
	{
		QVector3D tmp[2];
		tmp[0] = item->model * item->AABB_Box[0];
		tmp[1] = item->model * item->AABB_Box[1];
		AABB_Box[0].setX(qMax(AABB_Box[0].x(), tmp[0].x()));
		AABB_Box[0].setY(qMax(AABB_Box[0].y(), tmp[0].y()));
		AABB_Box[0].setZ(qMax(AABB_Box[0].z(), tmp[0].z()));
		AABB_Box[1].setX(qMin(AABB_Box[1].x(), tmp[1].x()));
		AABB_Box[1].setY(qMin(AABB_Box[1].y(), tmp[1].y()));
		AABB_Box[1].setZ(qMin(AABB_Box[1].z(), tmp[1].z()));
	}
}

void MyArrow::setUpModel()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	// 给缓存中存入普通箭头
	_VAO = new QOpenGLVertexArrayObject;
	_VBO = new QOpenGLBuffer;

	_VAO->create();
	_VBO->create();

	_VAO->bind();
	_VBO->bind();
	_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// 给顶点坐标和顶点颜色分配显存空间并写入顶点坐标
	_VBO->allocate(this->_vertices.constData(), sizeof(float) * (this->_vertices.size() + this->_colors.size()) * 3);
	// 写入顶点颜色
	_VBO->write(sizeof(float) * this->_vertices.size() * 3, this->_colors.constData(), sizeof(float) * this->_colors.size() * 3);

	// 顶点位置
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	// 顶点颜色
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(3 * sizeof(float) * this->_vertices.size()));
	_VAO->release();
	_VBO->release();

	// 给缓存中存入被选中箭头
	selected_VAO = new QOpenGLVertexArrayObject;
	selected_VBO = new QOpenGLBuffer;

	selected_VAO->create();
	selected_VBO->create();

	selected_VAO->bind();
	selected_VBO->bind();
	selected_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// 给顶点坐标和顶点颜色分配显存空间并写入顶点坐标
	selected_VBO->allocate(this->_vertices.constData(), sizeof(float) * (this->_vertices.size() + this->selected_move_arrow_colors.size()) * 3);
	// 写入顶点颜色
	selected_VBO->write(sizeof(float) * this->_vertices.size() * 3, this->selected_move_arrow_colors.constData(), sizeof(float) * this->selected_move_arrow_colors.size() * 3);

	// 顶点位置
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	// 顶点颜色
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(3 * sizeof(float) * this->_vertices.size()));
	selected_VAO->release();
	selected_VBO->release();
}
