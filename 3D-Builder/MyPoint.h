#pragma once
#include "ModelBase.h"

/**
 * \brief 点类，作为模型的一个成员变量，当模型有点被选中时将点信息加入点类缓冲中并绘制
 */
class MyPoint : public ModelBase
{
public:
	// 类中是否有点信息，决定点类是否绘制
	bool has_points = false;
	// 与基类中颜色不同的数据结构，与着色器的4通道相匹配
	MyPoint();
	void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const override;
	void setPoint(QVector3D point_vertex, QVector3D point_color);
	void clearPoints();
private:
	void setUpModel() override;
};
