#pragma once
#include "default.h"
#include "ModelBase.h"

/**
 * \brief 天空盒类，该类从文件中提取天空盒图像并作为1x1天空盒显示
 */
class SkyBox : public ModelBase
{
	Q_OBJECT
public:
	SkyBox();
	void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const override;
	void bindSkyBox(QOpenGLShaderProgram *program, int location);
private:
	void setUpModel() override;
	// 从文件中读取天空盒图像，其本质为加载图像成为立方体贴图
	void loadCubeMap();
};
