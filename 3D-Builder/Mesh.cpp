// #include "Mesh.h"
// #include <iostream>
//
// Mesh::Mesh()
// {
// 	
// }
//
// Mesh::Mesh(QVector<Vertex> vertices, QVector<unsigned> indices, QVector<Texture> textures)
// {
// 	this->vertices = vertices;
// 	this->_indices = indices;
// 	this->textures = textures;
// 	setUpModel();
// }
//
// void Mesh::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
// {
// 	// 如果调用绘制的是拾取阶段
// 	if (pick_mode)
// 	{
// 		program->bind();
// 		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
// 		program->setUniformValue("gMeshIndex", mesh_id);
// 		_VAO->bind();
// 		f->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
// 		_VAO->release();
// 		program->release();
// 		return;
// 	}
// 	unsigned int diffuseNr = 1;
// 	unsigned int specularNr = 1;
// 	unsigned int normalNr = 1;
// 	unsigned int heightNr = 1;
// 	program->bind();
// 	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
// 	for (unsigned int i = 0; i < textures.size(); i++)
// 	{
// 		// f->glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
// 		// 获取纹理序号（diffuse_textureN 中的 N）
// 		QString number;
// 		QString name = textures[i].type;
// 		if (name == "texture_diffuse")
// 			number = QString::number(diffuseNr++);
// 		else if (name == "texture_specular")
// 			number = QString::number(specularNr++);
// 		else if (name == "texture_normal")
// 			number = QString::number(normalNr++); // transfer unsigned int to stream
// 		else if (name == "texture_height")
// 			number = QString::number(heightNr++); // transfer unsigned int to stream
// 		QString str = "material." + name + number;
// 		program->setUniformValue(str.toStdString().c_str(), i);
// 		textures[i].texture->bind(i);
// 		// glBindTexture(GL_TEXTURE_2D, textures[i].id);
// 	}
// 	program->setUniformValue("alpha", this->alpha);
// 	program->setUniformValue("reflectivity", this->reflectivity);
// 	skybox->bindSkyBox(program, textures.size());
// 	// 绘制网格
// 	_VAO->bind();
// 	f->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
// 	_VAO->release();
//
// 	for (unsigned int i = 0; i < textures.size(); i++)
// 	{
// 		textures[i].texture->release();
// 	}
// 	program->release();
// }
//
// void Mesh::setUpModel()
// {
// 	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
// 	_VAO = new QOpenGLVertexArrayObject;
// 	_VBO = new QOpenGLBuffer;
// 	_EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//
// 	qDebug() << _indices;
// 	
// 	_VAO->create();
// 	_VBO->create();
// 	_EBO->create();
// 	
// 	_VAO->bind();
// 	_VBO->bind();
// 	_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
// 	_VBO->allocate(vertices.constData(), vertices.size() * sizeof(Vertex));
// 	_EBO->bind();
// 	_EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
// 	_EBO->allocate(_indices.constData(), _indices.size() * sizeof(unsigned int));
//
// 	// 顶点位置
// 	f->glEnableVertexAttribArray(0);
// 	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
// 	// 顶点法线
// 	f->glEnableVertexAttribArray(1);
// 	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
// 	// 顶点纹理坐标
// 	f->glEnableVertexAttribArray(2);
// 	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
// 	// 顶点颜色
// 	f->glEnableVertexAttribArray(3);
// 	f->glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colors));
// 	_VAO->release();
// 	_EBO->release();
// 	_VBO->release();
// }
//
//
