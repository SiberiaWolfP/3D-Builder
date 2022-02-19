#pragma once
#include <QtCore/QString>
#include <QtGui/QtGui>
#include "Mesh.h"
#include "MyPoint.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QtCore/QDebug>
#include <QtMath>
#include <limits>
#include "SkyBox.h"

QOpenGLTexture* TextureFromFile(QString path, const QString& directory, bool gamma = false);

// 单个顶点的数据
struct Vertex
{
	// 顶点坐标
	QVector3D Position;
	// 顶点法向量
	QVector3D Normal;
	// 顶点纹理坐标
	QVector2D TexCoords;
	// 顶点颜色
	QVector4D Colors;
	QVector3D Tangent;
	QVector3D Bitangent;
};

// 纹理
struct Texture
{
	// 纹理对象
	QOpenGLTexture* texture;
	// 纹理类型，分为漫反射纹理，镜面反射纹理等
	QString type;
	// 纹理文件路径
	QString path;
};

/**
 * \brief 模型类，该类从文件中读取模型数据并存入缓存，由Draw方法启动绘制
 */
class Model : public ModelBase
{
public:
	// 特殊的顶点和纹理数据，不能使用基类成员
	QVector<Vertex> vertices;
	QVector<Texture> textures;
	// 模型网孔，不是绘制出的面片，而是逻辑上的分段
	// QVector<Mesh> meshes;
	// 模型文件所在文件夹目录
	QString directory;
	// 模型文件所在绝对目录
	QString path;
	bool gammaCorrection;
	// 0位为碰撞盒XYZ最大值点，1位为最小值点
	QVector3D AABB_Box[2];
	// 模型碰撞盒中心点，用于“摆正”模型位置
	QVector3D view_Center;
	// 模型在x轴方向和y轴方向上碰撞盒大小的最大值
	float max_size;
	// 模型世界坐标变换矩阵，所有对模型位置的操作都相当于对该矩阵的操作
	QMatrix4x4 model;
	// 级联将模型平移、旋转和缩放的矩阵,org_model为将模型移到场景中心的矩阵
	QMatrix4x4 move_model, rotate_model, scale_model, org_model;
	// 记录模型中被选中的点
	MyPoint points;
	// 模型颜色
	QVector3D color;
	// 模型透明度
	float alpha = 1.0f;
	// 模型反光度
	float reflectivity = 0.0f;
	SkyBox* skybox;
	// 模型id
	int id;
	// 对象是否被选中
	bool is_selected = false;

	Model();

	Model(const Model& other)
		: textures(other.textures),
		  directory(other.directory),
		  path(other.path),
		  gammaCorrection(other.gammaCorrection),
		  view_Center(other.view_Center),
		  max_size(other.max_size),
		  model(other.model),
		  id(other.id),
		  is_selected(other.is_selected),
		  AABB_Box{other.AABB_Box[0], other.AABB_Box[1]}
	{
	}

	// 重载构造函数，根据指定路径加载文件中的模型
	Model(QString path, SkyBox* skybox, bool gamma = false);
	// 调用进行绘制，需传入当前绘制程序
	void Draw(QOpenGLShaderProgram* program, bool pick_mode = false) const override;
	// 绘制模型上被选中的顶点
	void DrawPoint(QOpenGLShaderProgram* program) const;
	// 判断该物体被选中的点是顶点还是面片上的点，若是顶点则将点加入点类中待绘制
	void judgeSelectedPoint(int meshID, float primitiveID,float vertexID);
	void setUpModel() override;
private:
	// 计算模型的碰撞盒，并以碰撞盒的中心作为模型逻辑中心
	// 由文件路径加载模型数据
	void loadModel(QString path);
	// 递归方法，递归获取模型每个node的数据
	void processNode(aiNode* node, const aiScene* scene);
	// 递归方法，递归获取每个node中mesh的数据
	void processMesh(aiMesh* mesh, const aiScene* scene);
	// 根据不同的纹理属性加载纹理，并存入模型纹理数组中，注意纹理不存进mesh类中
	QVector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, QString typeName);
};
