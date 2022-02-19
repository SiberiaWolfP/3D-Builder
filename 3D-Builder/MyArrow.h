#pragma once
#include "ModelBase.h"
#include "Model.h"

// 模型移动的六个方向
enum ModelMoveState
{
	Left,
	Right,
	Front,
	Back,
	Up,
	Down
};

enum ModelRotateState
{
	X_Forward,
	X_Reverse,
	Y_Forward,
	Y_Reverse,
	Z_Forward,
	Z_Reverse,
};

enum ModelScaleState
{
	X_Negative,
	X_Positive,
	Y_Negative,
	Y_Positive,
	Z_Negative,
	Z_Positive,
};

/**
 * \brief 箭头类，分别封装有选中和不选中箭头的数据。
 * \brief 对于移动箭头，每次只显示三个，根据摄像机所在象限调整箭头的显示位置
 */
class MyArrow : public ModelBase
{
	Q_OBJECT
public:
	// 被选中时的Arrow，颜色为黄色
	QVector<QVector3D> selected_move_arrow_colors;
	QOpenGLVertexArrayObject* selected_VAO;
	QOpenGLBuffer* selected_VBO;
	// 摄像机位置，根据该位置调整箭头们的位置
	QVector3D cameraPos;
	// 移动模式的model，与箭头框起来的物体保持一致
	QMatrix4x4 move_model;
	// 摄像机到箭头区域的向量
	QVector3D arrow_to_camera;
	// 箭头与摄像机距离，用于调整model矩阵，使得箭头相对大小保持不变
	float distance;
	// 框住被选中物体的碰撞盒，箭头绘制在碰撞盒外
	// 0位元素为最大点，1位元素为最小点
	QVector3D AABB_Box[2];
	QMap<int, Model*> selected_models;
	float arrow_distance = 0.075f;
	
	// 选中的箭头ID，0-5为移动箭头，6-11为旋转箭头，12-17为缩放箭头
	// 以下所有方向都是相对于摄像机看向Z轴负方向
	// 0,1：X轴（左右）移动箭头
	// 2,3：Y轴（上下）移动箭头
	// 4,5：Z轴（前后）移动箭头
	// 旋转箭头和缩放箭头在初始状态下在三轴方向
	// 6,7：左侧面右侧面旋转箭头
	// 8,9：顶面底面旋转箭头
	// 10,11：前面后面旋转箭头
	// 12,13：左右缩放箭头
	// 14,15：顶面底面缩放箭头
	// 16,17：前面后面缩放箭头
	int select_arrow_id;
	
	MyArrow();
	MyArrow(QVector3D cameraPos);
	virtual ~MyArrow();
	void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const override;
	void setArrowSelect(int arrowID);
	void moveArrow(ModelMoveState state);
	void modelsRotate(ModelRotateState state);
	void modelsScale(ModelScaleState state);
public slots:
	void updateCameraPos(QVector3D pos);
	void objSelected(Model *model);
	void objUnSelected(Model *model);
private:
	void setUpModel() override;
};
