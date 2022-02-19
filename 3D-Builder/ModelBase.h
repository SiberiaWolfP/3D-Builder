#pragma once
#include <QObject>
#include <QtOpenGLExtensions>

/**
 * \brief 模型基类，提供在OpenGL环境下各类模型可能共同需要的一些数据成员和操作
 */
class ModelBase : public QOpenGLContext
{
	Q_OBJECT
public:
	// 模型id
	int id;
	// 对象是否被选中
	bool is_selected = false;
	// 模型顶点
	QVector<QVector3D> _vertices;
	QVector<QVector3D> _colors;
	// 模型顶点索引
	QVector<unsigned int> _indices;
	// 模型纹理
	QOpenGLTexture* _texture;
	// 模型缓冲
	QOpenGLVertexArrayObject* _VAO;
	QOpenGLBuffer* _VBO, * _EBO;

	virtual ~ModelBase()
	{
	}
	// 调用进行绘制，需传入当前绘制程序
	virtual void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const = 0;
private:
	virtual void setUpModel() = 0;
};
