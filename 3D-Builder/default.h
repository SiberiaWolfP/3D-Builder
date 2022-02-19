#pragma execution_character_set("utf-8")
#pragma once
#ifndef __DEFAULT_H__
#define __DEFAULT_H__

// 路径头
#define RES_PATH "./Resources/"
#define SHADERS_PATH "./shaders/"
/*
 *	MainWindow
 */
#define WINDOW_QSS RES_PATH"theme.qss"
#define WINDOW_ICON RES_PATH"win.png"
#define WINDOW_TITLE "测试窗口"

#define WIN_DEFAULT_FONT_FAMILY "Microsoft YaHei UI"
#define WIN_DEFAULT_FONT_SIZE 1em

/*
 *	Title Bar 
 */
#define TITLE_BAR_HEIGHT 40
#define TITLE_BUTTON_SIZE 40
#define TITLE_LABEL_SIZE 40
#define TITLE_BUTTON_WIDTH 60
#define TITLE_BUTTON_HEIGHT 40
#define TITLE_ICON_MAG 0

#define APP_MAIN_COLOR 75,149,205

// 图片路径
#define TITLE_MIN_ICON RES_PATH"min.png"
#define TITLE_CLS_ICON RES_PATH"exit.png"
#define TITLE_RESTORE_ICON RES_PATH"restore.png"
#define MENU_BTN_ICON RES_PATH"menu-button.png"
#define CUBE_BTN_ICON RES_PATH"cube.png"
#define CYLINDER_BTN_ICON RES_PATH"Cylinder.png"
#define PYRAMID_BTN_ICON RES_PATH"pyramid.png"
#define INSERT_BTN_ICON RES_PATH"insert.png"
#define OBJECT_SELECT_BTN_ICON RES_PATH"select_1.png"
#define POINT_SELECT_BTN_ICON RES_PATH"select_2.png"
#define WIRE_FRAME_BTN_ICON RES_PATH"wireframe.png"
#define RABBIT_BTN_ICON RES_PATH"rabbit.png"
#define PICK_DEBUG_BTN_ICON RES_PATH"debug.png"
#define SELECT_COLOR_BTN_ICON RES_PATH"color_select.png"
#define SAVE_BTN_ICON RES_PATH"save.png"
#define RENDER_BTN_ICON RES_PATH"render.png"
#define SAVE_AS_BTN_ICON RES_PATH"saveas.png"
#define BALL_BTN_ICON RES_PATH"Ball.png"
#define CONE_BTN_ICON RES_PATH"Cone.png"
#define HEXAGON_BTN_ICON RES_PATH"hexagon.png"
#define RING_BTN_ICON RES_PATH"Ring.png"
#define TETRAHEDRON_BTN_ICON RES_PATH"tetrahedron.png"


// qss路径
#define FUNC_TAB_WIDGET_STYLE RES_PATH"FuncTabWidget.qss"
#define TAB_WIDGET_STYLE RES_PATH"TabWidgetStyle.qss"
#define TITLE_STYLE RES_PATH"TitleButtonStyle.qss"

// 着色器路径
#define SHADER_MODEL_VERT SHADERS_PATH"model_loading.vert"
#define SHADER_MODEL_GEOM SHADERS_PATH"model_loading.geom"
#define SHADER_MODEL_FRAG SHADERS_PATH"model_loading.frag"
#define SHADER_STENCIL_SINGLE_COLOR_VERT SHADERS_PATH"stencil_single_color.vs"
#define SHADER_STENCIL_SINGLE_COLOR_FRAG SHADERS_PATH"stencil_single_color.fs"
#define SHADER_SKYBOX_VERT SHADERS_PATH"skybox.vs"
#define SHADER_SKYBOX_FRAG SHADERS_PATH"skybox.fs"
#define SHADER_FLOOR_VERT SHADERS_PATH"floor.vs"
#define SHADER_FLOOR_FRAG SHADERS_PATH"floor.fs"
#define SHADER_PICK_VERT SHADERS_PATH"pick.vs"
#define SHADER_PICK_GEOM SHADERS_PATH"pick.gs"
#define SHADER_PICK_FRAG SHADERS_PATH"pick.fs"
#define SHADER_POINT_VERT SHADERS_PATH"points.vert"
#define SHADER_POINT_FRAG SHADERS_PATH"points.frag"
#define SHADER_ARROW_VERT SHADERS_PATH"arrow.vs"
#define SHADER_ARROW_GEOM SHADERS_PATH"arrow.gs"
#define SHADER_ARROW_FRAG SHADERS_PATH"arrow.fs"


// 模型路径
#define MODEL_NANOSUIT RES_PATH"nanosuit/nanosuit.obj"
#define MODEL_CUBE RES_PATH"Cube.json"
#define MODEL_CYLINDER RES_PATH"Cylinder.json"
#define MODEL_PYRAMID RES_PATH"Pyramid.json"
#define MODEL_BALL RES_PATH"Ball.json"
#define MODEL_CONE RES_PATH"Cone.json"
#define MODEL_HEXAGON RES_PATH"Hexagon.json"
#define MODEL_RING RES_PATH"Ring.json"
#define MODEL_WEDGE RES_PATH"Wedge.json"
#define MODEL_TETRAHEDRON RES_PATH"tetrahedron.json"
#define MODEL_BUNNY RES_PATH"bunny_iH.json"

// 天空盒路径
#define SKYBOX_MAP RES_PATH"skybox/"

extern float screen_relative_x_pix;
extern float screen_relative_y_pix;

#endif