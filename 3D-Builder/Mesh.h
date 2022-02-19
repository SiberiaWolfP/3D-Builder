// #pragma once
// #include <QtCore/QString>
// #include <QtGui/QVector3D>
// #include <QtOpenGLExtensions/QtOpenGLExtensions>
// #include <QtGui/QOpenGLFunctions_4_4_Core>
// #include "ModelBase.h"
// #include "SkyBox.h"
//
// // 单个顶点的数据
// // struct Vertex
// // {
// 	// 顶点坐标
// 	QVector3D Position;
// 	// 顶点法向量
// 	QVector3D Normal;
// 	// 顶点纹理坐标
// 	QVector2D TexCoords;
// 	// 顶点颜色
// 	QVector4D Colors;
// 	QVector3D Tangent;
// 	QVector3D Bitangent;
// };
//
// // 纹理
// struct Texture
// {
// 	// 纹理对象
// 	QOpenGLTexture* texture;
// 	// 纹理类型，分为漫反射纹理，镜面反射纹理等
// 	QString type;
// 	// 纹理文件路径
// 	QString path;
// };
//
//
// /**
//  * \brief 网格类，描述模型对象中一个网格（由文件规定的逻辑分段）的数据
//  */
// class Mesh : public ModelBase
// {
// public:
// 	// 特殊的顶点和纹理数据，不能使用基类成员
// 	QVector<Vertex> vertices;
// 	QVector<Texture> textures;
// 	// 模型颜色
// 	QVector3D color;
// 	// 模型透明度
// 	float alpha = 1.0f;
// 	// 模型反光度
// 	float reflectivity = 0.0f;
// 	SkyBox* skybox;
// 	// AABB碰撞盒，0位为碰撞盒XYZ坐标最大值，1位为碰撞盒XYZ坐标最小值
// 	QVector3D AABB_Box[2];
// 	// meshID
// 	int mesh_id;
// 	Mesh();
//
// 	Mesh(const Mesh& mesh)
// 	{
// 		if (this == &mesh) return;
// 		this->vertices = mesh.vertices;
// 		this->_indices = mesh._indices;
// 		this->textures = mesh.textures;
// 		this->color = mesh.color;
// 		this->alpha = mesh.alpha;
// 		this->reflectivity = mesh.reflectivity;
// 		this->skybox = mesh.skybox;
// 		this->mesh_id = mesh.mesh_id;
// 		this->AABB_Box[0] = mesh.AABB_Box[0];
// 		this->AABB_Box[1] = mesh.AABB_Box[1];
//
// 		this->_VAO = mesh._VAO;
// 		this->_VBO = mesh._VBO;
// 		this->_EBO = mesh._EBO;
// 	}
//
// 	Mesh& operator= (const Mesh &other)
// 	{
// 		if (this == &other) return *this;
// 		this->vertices = other.vertices;
// 		this->_indices = other._indices;
// 		this->textures = other.textures;
// 		this->color = other.color;
// 		this->alpha = other.alpha;
// 		this->reflectivity = other.reflectivity;
// 		this->skybox = other.skybox;
// 		this->mesh_id = other.mesh_id;
// 		this->AABB_Box[0] = other.AABB_Box[0];
// 		this->AABB_Box[1] = other.AABB_Box[1];
//
// 		this->_VAO = other._VAO;
// 		this->_VBO = other._VBO;
// 		this->_EBO = other._EBO;
// 		return *this;
// 	}
// 	// 由model类调用，初始化每个网格
// 	Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures);
// 	// 绘制，pick_mode决定该次绘制是拾取阶段还是正常阶段
// 	void Draw(QOpenGLShaderProgram* program, bool pick_mode) const override;
// private:
// 	// 将网格中保存在内存的顶点数据存入显存，加快渲染速度
// 	void setUpModel() override;
// };