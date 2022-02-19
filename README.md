# 3D-Builder

需求规格说明书: https://github.com/3D-Builder-Team/3D-Builder/blob/master/docs/Demand_Analysis/%E9%9C%80%E6%B1%82%E8%A7%84%E6%A0%BC%E8%AF%B4%E6%98%8E%E4%B9%A6.md

系统设计文档: https://github.com/3D-Builder-Team/3D-Builder/blob/master/docs/System_Design/System_Design.md

测试文档: https://github.com/3D-Builder-Team/3D-Builder/blob/master/docs/Test_Documentation/Test%20Documentation.md

## 12.20 更新

### author: 张永康

在/Raytracing_Render/目录下，加入基于CUDA并行加速的光线追踪渲染器代码

由于CUDA代码需要使用NVIDIA的NVCC编译，因此该部分开发过程和前端(3D Builder)分开了，具体迭代细节参见：https://github.com/qpswwww/Raytracing_CUDA

## 12.12 更新

### author: Siber

添加了三轴移动箭头，添加了对透明和镜面反射天空盒的支持

## 12.4 更新

### author: Siber

彻底完善了点选取

## 12.3 更新

### author: Siber

添加线框模式，在View选项卡下设置开关

重新加回了对象边框的绘制

## 12.2 更新

### author: Siber

实现了对对象、对象中的逻辑分段、逻辑分段中的顶点标号的拾取

添加了由键盘简单移动已拾取的对象的功能：

注意：模型的上下移动对应键盘Shift和Ctrl，这两个键无法持续移动，需多次点击

重构了代码，添加了模型基类

添加了各头文件中成员变量和函数的功能说明(摄像机除外)

## 11.28 更新

添加了简易的网格地板，该地板顶点和颜色数据由代码生成，之后可以优化为可以使用摄像机距离动态调整网格大小。

## 11.27 更新

添加了“添加”按钮和几个测试用添加内部模型按钮，现在可以手动载入外部或内部模型。优化了内部模型数据结构，使用链表组织各模型。下阶段主要需求：

1. 物体拾取
2. 三维操作箭头的放置，因多物体时需处理分开和合并情况的碰撞盒处理
3. 物体被选取时的描边处理，主要问题在于物体被遮挡时正确地绘制物体边框
3. 其他3D-Builder功能

## 11.19 更新

 上传需求规格说明书V1.0

## 11.17 更新

完善摄像机，模式为轨迹球，使用四元数进行旋转。实现了预期的全部功能。当摄像机旋转到上下两极点时的限制仍不够自然，需要进一步调整。源码见Camera_in.h，Camera.h暂时不用。

额外添加了些小功能。添加了天空盒以及Debug页，可以实时调整光线参数。

## 11.9 更新

照抄3DBuilder做了个功能界面

优化了资源路径，目前使用default.h中宏定义

## 编译说明：

在VS中的项目属性中修改VC++包含目录和库目录，将第三方库和Qt库路径设置正确

## 第三方库说明：

- include下assimp库：可导入大量3D模型和导出数种主流模型文件的库
- lib下assimp-*文件：assimp库对应的动态链接库，必须添加到程序根目录
- lib下zlib.dll文件：assimp库使用到的压缩库，必须添加到程序根目录
