#pragma once
#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <QtGui/QtGui>
#include <QtOpenGL/QtOpenGL>
#include "Camera.h"
#include "Model.h"
#include "SkyBox.h"
#include "MyFloor.h"
#include "default.h"
#include "PickingTexture.h"
#include "MyArrow.h"
#include "FileReader.h"

// 辅助函数,用于进行哈希
typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

class QOpenGLShaderProgram;
/**
 * \brief OpenGL窗口，继承有Qt提供的OpenGL上下文API
 */
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Core
{
	Q_OBJECT
public:
	explicit MyOpenGLWidget(QWidget* parent = nullptr);
	~MyOpenGLWidget();
	// 对给定字符串进行散列
	static hash_t _hash_(char const* str)
	{
		hash_t ret{ basis };

		while (*str) {
			ret ^= *str;
			ret *= prime;
			str++;
		}

		return ret;
	}
	// C++11新特性，可将字符串用于case
	static constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
	{
		return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
	}

// 槽函数，可由信号激活
public slots:
	// 添加内置模型，如正方体、圆柱等
	void insertInsideModel();
	// 由文件载入模型
	void loadModel(QString path);
	// 以下三个函数为debug用，可设置光线属性
	void setAmbient(double value);
	void setDiffuse(double value);
	void setSpecular(double value);
	// 可以对功能按钮进行响应，设置程序的各种功能开关
	void setFuncAble(bool value);
	void colorSelected(QColor color);
	void alphaChanged(int value);
	void reflectivityChanged(int value);
	void saveModels(QString path);
protected:
	// 初始化OpenGL环境
	void initializeGL() override;
	// 绘制一帧
	void paintGL() override;
	// 当窗口大小变化时调用，实测无效，疑为历史遗留
	void resizeGL(int w, int h) override;
	// 当键盘上有按键按下时
	void keyPressEvent(QKeyEvent* event) override;
	// 当鼠标滚轮滚动时
	void wheelEvent(QWheelEvent* event) override;
	// 当鼠标按键按下并移动时
	void mouseMoveEvent(QMouseEvent* event) override;
	// 当鼠标按键按下时
	void mousePressEvent(QMouseEvent* event) override;
	// 当鼠标按键释放时
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	// 模型数组，存放各模型对象的数据
	QMap<int, Model*> ourModel;
	// QList<Model> ourModel;
	// 天空盒和地板模型
	SkyBox* skybox;
	MyFloor* floor;
	MyPoint* point;
	// 移动选择箭头
	MyArrow* arrow;
	// 渲染程序
	QOpenGLShaderProgram* program, * program_single_color,
		* program_skybox, * program_floor, * program_pick, * program_point,
		* program_arrow;
	// 上一次鼠标事件的坐标
	QPointF lastMousePos;

	// 用于记录两帧之间的时间间隔，可根据间隔动态调整摄像机移动速率以使移动平滑
	float deltaTime = 0.0f;
	qint64 lastTime = 0;

	// 光默认强度
	float ambient_strength = 0.7f;
	float diffuse_strength = 0.2f;
	float specular_strength = 0.1f;

	// 摄像机，注意，摄像机无需OpenGL环境，天空盒和地板需要在OpenGL下初始化并缓存数据，
	// 所以需定义为指针并在OpenGL环境初始化后再实例化
	Camera m_camera;
	// 顶点拾取，封装了对自定义帧缓冲的操作
	PickingTexture *m_picking_texture;

	// 指示对象是否可被选取
	bool object_select_able = true;
	// 指示对象上的点是否可被选取，该功能无需对象选取启用，即拾取点不代表拾取对象
	bool point_select_able = false;
	// 绘制线框功能是否启用
	bool wire_frame_able = false;
	// 选取Debug模式是否启用
	bool pick_debug_able = false;
	// 指示当前选中的是否是移动箭头，用于在鼠标拖动时判断移动物体还是旋转摄像头
	bool move_mode = false;
	// 按键功能组，1为平移，2为旋转，3为缩放
	int key_func = 1;
	
	// 初始化渲染程序
	void initProgram();
	// 初始化摄像机
	void initCamera();
	// 显示OpenGL信息
	void printContextInformation();
};

// 运算符重载，重载字符串运算符
constexpr unsigned long long operator "" _hash(char const* p, size_t)
{
	return MyOpenGLWidget::hash_compile_time(p);
}