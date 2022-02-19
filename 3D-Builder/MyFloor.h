#pragma once
#include <QtOpenGLExtensions>
#include "ModelBase.h"

/**
 * \brief 一个由灰色和白色网格组成的地板，模型数据由程序动态生成
 */
class MyFloor : public ModelBase
{
	Q_OBJECT
public:
	MyFloor();
	void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const override;
public slots:
	// 当摄像机位置改变时调用此函数，此后将使用摄像机位置动态调整地板大小
	void cameraPosChanged(QVector3D pos);
private:
	void setUpModel() override;

	// 网格大小
	float grid_step;
	// 地板大小
	float grid_range;
	// 摄像机位置
	QVector3D cameraPos;
};
