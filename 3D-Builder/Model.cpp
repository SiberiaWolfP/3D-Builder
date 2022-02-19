#include "Model.h"

Model::Model()
{
	
}

Model::Model(QString path, SkyBox* skybox, bool gamma)
{
	const int index = path.lastIndexOf('/');
	directory = path.mid(0, index + 1);
	this->path = path;
	this->skybox = skybox;

	this->color = { 1.0f, 1.0f, 1.0f };
	this->alpha = 1.0f;
	this->reflectivity = 0.0f;
	
	// Mesh mesh;
	// meshes.push_back(mesh);
	// meshes[0].skybox = skybox;
	// for (int i = 0; i < meshes.size(); i++)
	// {
		// meshes[i].skybox = skybox;
	// }
}

void Model::Draw(QOpenGLShaderProgram* program, bool pick_mode) const
{
	// for (int i = 0; i < meshes.size(); i++)
	// {
		// meshes[i].Draw(program, pick_mode);
	// }

	// 如果调用绘制的是拾取阶段
	if (pick_mode)
	{
		program->bind();
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		program->setUniformValue("gMeshIndex", 0);
		_VAO->bind();
		f->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
		_VAO->release();
		program->release();
		return;
	}
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	program->bind();
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// f->glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
		// 获取纹理序号（diffuse_textureN 中的 N）
		QString number;
		QString name = textures[i].type;
		if (name == "texture_diffuse")
			number = QString::number(diffuseNr++);
		else if (name == "texture_specular")
			number = QString::number(specularNr++);
		else if (name == "texture_normal")
			number = QString::number(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = QString::number(heightNr++); // transfer unsigned int to stream
		QString str = "material." + name + number;
		program->setUniformValue(str.toStdString().c_str(), i);
		textures[i].texture->bind(i);
		// glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	program->setUniformValue("alpha", this->alpha);
	program->setUniformValue("reflectivity", this->reflectivity);
	program->setUniformValue("customColors", this->color);
	skybox->bindSkyBox(program, textures.size());
	// 绘制网格
	_VAO->bind();
	f->glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	_VAO->release();

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i].texture->release();
	}
	program->release();
}

void Model::DrawPoint(QOpenGLShaderProgram* program) const
{
	this->points.Draw(program);
}

void Model::judgeSelectedPoint(int meshID, float primitiveID, float vertexID)
{
	qDebug() << "object " << id << "select a vertex:" << vertexID;
	// 经管线输出插值后的顶点ID
	// 经过四舍五入后该点较为靠近的顶点ID
	int true_id_int = (int)(vertexID + 0.5f);
	// 在OpenGL中，DrawElement调用后产生的内部索引为indices数组中的元素
	this->points.setPoint(this->vertices[true_id_int].Position, { 0.0f,0.4784f,0.8f });
}

void Model::setUpModel()
{
	this->max_size = 1.0f;
	// 遍历每个网格的碰撞盒，求出总的模型的碰撞盒，进而求出模型视觉中心
	const float max = FLT_MAX / 2;
	const float min = -max;
	AABB_Box[0] = { min, min,min };
	AABB_Box[1] = { max, max,max };
	for (int i = 0; i < vertices.size(); i++)
	{
		AABB_Box[0].setX(qMax(AABB_Box[0].x(), vertices[i].Position.x()));
		AABB_Box[0].setY(qMax(AABB_Box[0].y(), vertices[i].Position.y()));
		AABB_Box[0].setZ(qMax(AABB_Box[0].z(), vertices[i].Position.z()));
		AABB_Box[1].setX(qMin(AABB_Box[1].x(), vertices[i].Position.x()));
		AABB_Box[1].setY(qMin(AABB_Box[1].y(), vertices[i].Position.y()));
		AABB_Box[1].setZ(qMin(AABB_Box[1].z(), vertices[i].Position.z()));
	}
	this->view_Center = (AABB_Box[0] + AABB_Box[1]) / 2;

	float x_max = AABB_Box[0].x() - AABB_Box[1].x();
	float y_max = AABB_Box[0].y() - AABB_Box[1].y();
	this->max_size = qMax(x_max, y_max);

	// loadModel(path);
	model.setToIdentity();
	move_model.setToIdentity();
	rotate_model.setToIdentity();
	scale_model.setToIdentity();
	// model.scale(40 / this->max_size);
	// 将对象移动到坐标中心的地板之上
	org_model.translate(-view_Center);
	move_model.scale(140 / this->max_size);
	move_model.translate(0, y_max / 2.0f, 0);
	// model.translate(-view_Center);
	model = move_model * org_model;

	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	_VAO = new QOpenGLVertexArrayObject;
	_VBO = new QOpenGLBuffer;
	_EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	// 将顶点数据绑定到显卡显存
	_VAO->create();
	_VBO->create();
	_EBO->create();

	_VAO->bind();
	_VBO->bind();
	_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	_VBO->allocate(vertices.constData(), vertices.size() * sizeof(Vertex));
	_EBO->bind();
	_EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	_EBO->allocate(_indices.constData(), _indices.size() * sizeof(unsigned int));

	// 顶点位置
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// 顶点法线
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// 顶点纹理坐标
	f->glEnableVertexAttribArray(2);
	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// 顶点颜色
	f->glEnableVertexAttribArray(3);
	f->glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colors));
	_VAO->release();
	_EBO->release();
	_VBO->release();
}

void Model::loadModel(QString path)
{
	// Assimp::Importer import;
	// unsigned int steps = aiProcess_Triangulate;
	// steps |= aiProcess_FlipUVs;
	// steps |= aiProcess_CalcTangentSpace;
	// // steps |= aiProcess_GenSmoothNormals;
	// steps |= aiProcess_GenNormals;
	// steps |= aiProcess_FixInfacingNormals;
	// steps |= aiProcess_GenBoundingBoxes;
	// const aiScene* scene = import.ReadFile(path.toStdString().c_str(), steps);

	// if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	// {
		// qDebug() << "ERROR:ASSIMP:: " << import.GetErrorString();
		// return;
	// }
	
	// const int index = path.lastIndexOf('/');
	// directory = path.mid(0, index + 1);
	// this->path = path;

	// processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
		// meshes.push_back(processMesh(mesh, scene));
		// meshes.last().mesh_id = meshes.size() - 1;
		this->AABB_Box[0] = { mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z };
		this->AABB_Box[1] = { mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z };
		// meshes.last().AABB_Box[0] = { mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z };
		// meshes.last().AABB_Box[1] = { mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z };
	}
	// 对该节点的子节点进行处理
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 顶点坐标
		QVector3D vector = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.Position = vector;
		// 法线
		if (mesh->HasNormals() == true)
		{
			vector = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			vertex.Normal = vector;
		} else
		{
			vertex.Normal = { 0.0f, 0.0f, 0.0f };
		}
		// 纹理坐标
		if (mesh->mTextureCoords[0])
		{
			QVector2D vec;
			vec = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = { 0.0f, 0.0f };
		}
		// 顶点颜色
		if (mesh->HasVertexColors(0))
		{
			QVector4D color;
			color = { mesh->mColors[0][i].r / float(255), mesh->mColors[0][i].g / float(255), mesh->mColors[0][i].b / float(255), mesh->mColors[0][i].a / float(255) };
			vertex.Colors = color;
		} else
		{
			vertex.Colors = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		// tangent
		if (mesh->HasTangentsAndBitangents() == true)
		{
			vector = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			vertex.Tangent = vector;
			// bitangent
			vector = { mesh->mBitangents[i].x,mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			vertex.Bitangent = vector;
		} else
		{
			vertex.Tangent = { 0.0f, 0.0f, 0.0f };
			vertex.Bitangent = { 0.0f, 0.0f, 0.0f };
		}
		this->vertices.push_back(vertex);
	}
	// 索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			this->_indices.push_back(face.mIndices[j]);
		}
	}
	// 材质
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0
			&& material->GetTextureCount(aiTextureType_SPECULAR) == 0
			&& material->GetTextureCount(aiTextureType_HEIGHT) == 0
			&& material->GetTextureCount(aiTextureType_AMBIENT) == 0)
		{
			goto genDefaultTexture;
		}
		QVector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		this->textures.append(diffuseMaps);
		QVector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		this->textures.append(specularMaps);
		QVector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		this->textures.append(normalMaps);
		QVector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		this->textures.append(heightMaps);
	} else 
	{
		genDefaultTexture:
		QImage image(1, 1, QImage::Format_ARGB32_Premultiplied);
		QPainter painter(&image);
		painter.fillRect(image.rect(), Qt::white);
		QVector<Texture> DefaultMaps;
		Texture texture;
		texture.texture = new QOpenGLTexture(image);
		texture.path = "";
		texture.type = "texture_diffuse";
		DefaultMaps.append(texture);
		this->textures.append(texture);
		texture.type = "texture_specular";
		DefaultMaps.clear();
		this->textures.append(texture);
	}
}

QVector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, QString typeName)
{
	QVector<Texture> new_textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		// 在已加载纹理中搜索当前需加载的纹理是否已加载过，以防重复加载
		for (unsigned int j = 0; j < textures.size(); j++)
		{
			if (textures[j].path.compare(str.C_Str()) == 0)
			{
				new_textures.push_back(textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.texture = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			new_textures.push_back(texture);
			textures.push_back(texture);
		}
	}
	return new_textures;
}

QOpenGLTexture* TextureFromFile(QString path, const QString& directory, bool gamma)
{
	QString filename = QString(path);
	filename = directory + '/' + filename;

	// 加载纹理贴图
	QOpenGLTexture* texture = new QOpenGLTexture(QImage(filename));
	if (!texture->isCreated()) qDebug() << "Failed to load texture" << " path: " << path;
	// 设置环绕方式
	texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
	texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
	// 设置过滤方式，缩小时为邻近，放大时为线性
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	// texture->setWrapMode(QOpenGLTexture::Repeat);
	return texture;
}