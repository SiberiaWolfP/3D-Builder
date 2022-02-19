#include "PickingTexture.h"
#include <QDialog>
#include <QLabel>

PickingTexture::PickingTexture()
{
}

PickingTexture::~PickingTexture()
{
	delete FBO;
}

bool PickingTexture::Init(unsigned window_width, unsigned window_height)
{
	// 获取当前OpenGL环境
	this->doneCurrent();
	// 设置帧缓冲格式，内部数据为RGBA四通道，每通道32位并为浮点数，并附加深度缓存
	// 注意：若无深度缓冲则无法正确获取到前方的点
	QOpenGLFramebufferObjectFormat format;
	format.setInternalTextureFormat(GL_RGBA32F);
	format.setAttachment(QOpenGLFramebufferObject::Depth);
	FBO = new QOpenGLFramebufferObject(window_width, window_height, format);
	FBO->bind();
	// 环境中绑定为默认缓冲
	FBO->bindDefault();
	// 检测FBO是否正确生成
	if (!FBO->isValid())
	{
		qDebug() << "FBO is not valid, pay attention!";
		return false;
	}

	return true;
}

void PickingTexture::EnableWriting()
{
	// 将输出绑定到自定义缓冲
	this->doneCurrent();
	FBO->bind();
}

void PickingTexture::DisableWriting()
{
	// 将输出绑定到默认缓冲
	this->doneCurrent();
	FBO->bindDefault();
}

PickingTexture::PixelInfo PickingTexture::ReadPixel(unsigned x, unsigned y)
{
	this->doneCurrent();
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	PixelInfo Pixel;
	FBO->bind();
	// 从自定义缓冲中根据坐标获取指定位置的数据
	f->glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &Pixel);
	FBO->bindDefault();
	return Pixel;
}
