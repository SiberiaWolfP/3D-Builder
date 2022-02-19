#pragma once
#include <QtOpenGLExtensions>
#include <QOpenGLFunctions_3_1>

/**
 * \brief 该类中封装有一个自定义帧缓冲，用于拾取
 */
class PickingTexture : public QOpenGLContext
{
public:
	PickingTexture();
	~PickingTexture();
	// 初始化自定义帧缓存，若帧缓存已存在，则删除之前的新建帧缓存
	bool Init(unsigned int window_width, unsigned int window_height);
	// 设置管线输出到自定义缓存中
	void EnableWriting();
	// 设置管线输出到默认缓存
	void DisableWriting();
	// 从缓存中读出的数据
	struct PixelInfo
	{
		// 对象ID
		float ObjectID;
		// 对象中的meshID
		float MeshID;
		// 绘制的图元ID
		float PrimID;
		// 经过插值后的顶点ID
		float vertID;
		
		PixelInfo()
		{
			ObjectID = 0;
			MeshID = 0;
			PrimID = 0;
			vertID = 0;
		}
	};
	// 对给定的坐标读取相应位置上的数据
	PixelInfo ReadPixel(unsigned int x, unsigned int y);
private:
	// 帧缓存
	QOpenGLFramebufferObject* FBO;
};
