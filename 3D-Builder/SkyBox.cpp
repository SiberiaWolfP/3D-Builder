#include "SkyBox.h"

SkyBox::SkyBox()
{
	this->_vertices.append(
		{ { -1.0f, 1.0f, -1.0f },
		{-1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f },
		{1.0f, 1.0f, -1.0f },
		{-1.0f, 1.0f, -1.0f },

		{-1.0f, -1.0f, 1.0f },
		{-1.0f, -1.0f, -1.0f },
		{-1.0f, 1.0f, -1.0f },
		{-1.0f, 1.0f, -1.0f },
		{-1.0f, 1.0f, 1.0f },
		{-1.0f, -1.0f, 1.0f },

		{1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, 1.0f },
		{1.0f, 1.0f, 1.0f },
		{1.0f, 1.0f, 1.0f },
		{1.0f, 1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f },

		{-1.0f, -1.0f, 1.0f },
		{-1.0f, 1.0f, 1.0f },
		{1.0f, 1.0f, 1.0f },
		{1.0f, 1.0f, 1.0f },
		{1.0f, -1.0f, 1.0f },
		{-1.0f, -1.0f, 1.0f },

		{-1.0f, 1.0f, -1.0f },
		{1.0f, 1.0f, -1.0f },
		{1.0f, 1.0f, 1.0f },
		{1.0f, 1.0f, 1.0f },
		{-1.0f, 1.0f, 1.0f },
		{-1.0f, 1.0f, -1.0f },

		{-1.0f, -1.0f, -1.0f },
		{-1.0f, -1.0f, 1.0f },
		{1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f },
		{-1.0f, -1.0f, 1.0f },
		{1.0f, -1.0f, 1.0f } }
	);
	loadCubeMap();
	setUpModel();
}

void SkyBox::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
{
	// program->bind();
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	program->setUniformValue("skybox", 0);
	_texture->bind(0);
	_VAO->bind();
	f->glDepthFunc(GL_LEQUAL);
	f->glDrawArrays(GL_TRIANGLES, 0, 36);
	f->glDepthFunc(GL_LESS);
	_VAO->release();
	// program->release();
}

void SkyBox::bindSkyBox(QOpenGLShaderProgram* program, int location)
{
	program->bind();
	program->setUniformValue("skybox", location);
	_texture->bind(location);
	// program->release();
}

void SkyBox::setUpModel()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	_VAO = new QOpenGLVertexArrayObject;
	_VBO = new QOpenGLBuffer;

	_VAO->create();
	_VBO->create();

	_VAO->bind();
	_VBO->bind();
	_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	_VBO->allocate(this->_vertices.constData(), this->_vertices.size() * sizeof(float) * 3);

	// 顶点位置
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	_VAO->release();
	_VBO->release();
}

void SkyBox::loadCubeMap()
{
	const QImage front = QImage(SKYBOX_MAP"front.jpg").convertToFormat(QImage::Format_RGBA8888);
	const QImage back = QImage(SKYBOX_MAP"back.jpg").convertToFormat(QImage::Format_RGBA8888);
	const QImage left = QImage(SKYBOX_MAP"left.jpg").convertToFormat(QImage::Format_RGBA8888);
	const QImage right = QImage(SKYBOX_MAP"right.jpg").convertToFormat(QImage::Format_RGBA8888);
	const QImage top = QImage(SKYBOX_MAP"top.jpg").convertToFormat(QImage::Format_RGBA8888);
	const QImage bottom = QImage(SKYBOX_MAP"bottom.jpg").convertToFormat(QImage::Format_RGBA8888);
	QVector<QImage> faces;
	faces.append({ right, left, top, bottom, front, back });

	_texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
	_texture->create();
	_texture->setSize(front.width(), front.height(), front.depth());
	_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
	_texture->allocateStorage();
	for (int i = 0; i < 6; i++)
	{
		_texture->setData(0, 0, QOpenGLTexture::CubeMapFace(QOpenGLTexture::CubeMapPositiveX + i), 
			QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8,
			static_cast<const void*>(faces[i].constBits()), nullptr);
	}
	_texture->generateMipMaps();
	_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
	_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
}
