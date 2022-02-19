#include "MyFloor.h"

MyFloor::MyFloor()
{
	grid_step = 40.0f;
	grid_range = 500.0f;
	int m = 0, n = 0;
	for (float x = -grid_range; x <= grid_range; x += grid_step)
	{
		n = 0;
		for (float z = -grid_range; z <= grid_range; z += grid_step)
		{
			if ((n % 2 == 0 && m % 2 == 0) || (n % 2 == 1 && m % 2 == 1))
			{
				this->_vertices.append({
					{x, 0, z},
					{x + grid_step, 0, z + grid_step},
					{x + grid_step, 0, z},
					{x, 0, z},
					{x, 0, z + grid_step},
					{x + grid_step, 0, z + grid_step} });
				this->_colors.append({
					{0.870588f, 0.870588f, 0.870588f},
					{0.870588f, 0.870588f, 0.870588f},
					{0.870588f, 0.870588f, 0.870588f},
					{0.870588f, 0.870588f, 0.870588f},
					{0.870588f, 0.870588f, 0.870588f},
					{0.870588f, 0.870588f, 0.870588f} });
			} else if (n % 2 == 1 && m % 2 == 0 || (n % 2 == 0 && m % 2 == 1))
			{
				this->_vertices.append({
					{x, 0, z},
					{x + grid_step, 0, z + grid_step},
					{x + grid_step, 0, z},
					{x, 0, z},
					{x, 0, z + grid_step},
					{x + grid_step, 0, z + grid_step} });
				this->_colors.append({
					{0.78431f, 0.78431f, 0.78431f},
					{0.78431f, 0.78431f, 0.78431f},
					{0.78431f, 0.78431f, 0.78431f},
					{0.78431f, 0.78431f, 0.78431f},
					{0.78431f, 0.78431f, 0.78431f},
					{0.78431f, 0.78431f, 0.78431f} });
			}
			n++;
		}
		m++;
	}
	setUpModel();
}

void MyFloor::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	program->bind();
	_VAO->bind();
	// f->glDepthFunc(GL_LEQUAL);
	f->glDrawArrays(GL_TRIANGLES, 0, this->_vertices.size());
	// f->glDepthFunc(GL_LESS);
	_VAO->release();
	program->release();
}

void MyFloor::cameraPosChanged(QVector3D pos)
{
	this->cameraPos = pos;
}

void MyFloor::setUpModel()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	_VAO = new QOpenGLVertexArrayObject;
	_VBO = new QOpenGLBuffer;
	// EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	_VAO->create();
	_VBO->create();
	// EBO->create();

	_VAO->bind();
	_VBO->bind();
	_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// 给顶点坐标和顶点颜色分配显存空间并写入顶点坐标
	_VBO->allocate(this->_vertices.constData(), sizeof(float) * (this->_vertices.size() + this->_colors.size()) * 3);
	// 写入顶点颜色
	_VBO->write(sizeof(float) * this->_vertices.size() * 3, this->_colors.constData(), sizeof(float) * this->_colors.size() * 3);
	// EBO->bind();
	// EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// EBO->allocate(indices.constData(), indices.size() * sizeof(unsigned int));

	// 顶点位置
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	// 顶点颜色
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(3 * sizeof(float) * this->_vertices.size()));
	_VAO->release();
	// EBO->release();
	_VBO->release();
}
