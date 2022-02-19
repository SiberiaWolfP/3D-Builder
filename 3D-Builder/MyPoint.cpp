#include "MyPoint.h"
#include <QOpenGLFunctions_4_4_Core>

MyPoint::MyPoint()
{
	_VAO = new QOpenGLVertexArrayObject;
	_VBO = new QOpenGLBuffer;
}

void MyPoint::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
{
	program->bind();
 	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	QOpenGLFunctions_4_4_Core* functions_4_4_core = currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
	_VAO->bind();
	functions_4_4_core->glPointSize(10.0f);
	f->glDrawArrays(GL_POINTS, 0, this->_vertices.size());
	_VAO->release();
	program->release();
}

void MyPoint::setPoint(QVector3D point_vertex, QVector3D point_color)
{
	has_points = true;
	_VAO->bind();
	this->_vertices.append(point_vertex);
	this->_colors.append(point_color);
	_VAO->destroy();
	_VBO->destroy();
	setUpModel();
	_VAO->release();
}

void MyPoint::clearPoints()
{
	has_points = false;
	_VAO->bind();
	this->_vertices.clear();
	this->_colors.clear();
	_VAO->destroy();
	_VBO->destroy();
	_VAO->release();
}

void MyPoint::setUpModel()
{
	// 获取当前OpenGL环境函数指针
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

	_VAO->create();
	_VBO->create();

	//绑定顶点缓冲到当前上下文中
	_VAO->bind();
	_VBO->bind();
	_VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	
	_VBO->allocate(this->_vertices.constData(), this->_vertices.size() * sizeof(float) * 6);
	_VBO->write(sizeof(float) * this->_vertices.size() * 3, this->_colors.constData(), sizeof(float) * this->_colors.size() * 3);

	// 顶点位置
	// 激活渲染程序中的对应输入变量
	f->glEnableVertexAttribArray(0);
	// 将输入变量指针和缓冲区链接起来
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(sizeof(float) * this->_vertices.size() * 3));
	
	_VAO->release();
	_VBO->release();
}
