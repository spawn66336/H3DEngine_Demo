/**
* @file render_system.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "game_graphic_header.h"

class CCameraEntity;
class CGraphicsResMng;
class CPrePassLightEntity;

enum WINDOW_MODE
{
	WINDOW_NORMAL,
	WINDOW_FULL_SCREEN
};

typedef H3DI::IRender* (*CreateRenderPtr)();
typedef void (*DeleteRenderPtr)();
typedef ISpecialEffectManager* (*CreateISpecialEffectManager)(H3DI::IRender*);
typedef void (*DeleteISpecialEffectManager)(ISpecialEffectManager*);

class CRenderSystem
{
public:
	CRenderSystem(CGraphicsResMng* mng);
	virtual ~CRenderSystem();

	/**
	* \brief
	* 初始化
	* 
	* \returns
	* 如果成功返回1, 否则返回0
	*/
	virtual bool initialize(H3DI::IRender* irender);

	/**
	* \brief
	* 设置各异向性过滤
	*
	* \param afmode
	* 各异向性过滤参数
	*/
	void set_af_mode(H3DI::EAFMode afMode);

	/**
	* \brief
	* 获得各异向性过滤
	*
	* \returns
	* 各异向性过滤参数
	*/
	H3DI::EAFMode get_af_mode() const;

	/**
	* \brief
	* 设置反锯齿
	*
	* \param afmode
	* 反锯齿参数
	*/
	void set_aa_mode(H3DI::EAAMode aaMode);

	/**
	* \brief
	* 获得反锯齿
	*
	* \returns
	* 反锯齿参数
	*/
	H3DI::EAAMode set_aa_mode() const;

	/**
	* \brief
	* 设置屏幕清除标志位
	*
	* 参数可以由以下参数的结合(通过|结合), 
	* CLEAR_BUF_COLOR = 1
	* CLEAR_BUF_DEPTH = 2
	* CLEAR_BUF_STENCIL = 4
	*
	* \param flags
	* 清除标志
	*/
	void set_clear_flags(unsigned int flags);

	/**
	* \brief
	* 获得屏幕清除标志
	*
	* \returns
	* 清除标志
	*/
	unsigned int get_clear_flags() const;


	/**
	* \brief
	* 设置是否使用三角形列表
	*
	* \param use
	* 是否使用三角形列表
	*/
	void set_use_triangle_list(bool use);

	/**
	* \brief
	* 获得是否使用三角形列表
	*
	* \returns
	* 是否使用三角形列表
	*/
	bool get_use_triangle_list() const;

	/**
	* \brief
	* 设置是否使用垂直同步
	*
	* \param veticalSync
	* 是否使用垂直同步
	*/
	void set_vetical_sync(bool veticalSync);

	/**
	* \brief
	* 获得垂直同步标志
	*
	* \returns
	* 是否使用垂直同步
	*/
	bool get_vetical_sync() const;

	/**
	* \brief
	* 开始渲染
	*
	* 每次渲染之前都要先调用这个函数
	*
	*/
	void begin_render();

	/**
	* \brief
	* 结束渲染
	*
	* 每次渲染结束都要先调用这个函数
	*
	*/
	void end_render();

	/**
	* \brief
	* 保存屏幕到指定文件
	*
	* \param fileName
	* 图片文件保存名字
	*/
	void save_screen_to_file(const std::string& fileName);

	/**
	* \brief
	* 获得支持的最大af值
	*
	* \returns
	* af值
	*/
	H3DI::EAFMode get_supported_af_mode();

	/**
	* \brief
	* 获得支持的最大aa值
	*
	* \returns
	* aa值
	*/
	H3DI::EAAMode get_supported_aa_mode();

	/**
	* \brief
	* 设置窗口模式, 可以是全屏, 或者正常窗口
	*
	* \param windowMode
	* 窗口模式
	*/
	void set_window_mode(WINDOW_MODE windowMode);

	/**
	* \brief
	* 计算3d空间中位置在电脑屏幕坐标系下的位置
	* 
	* \param objectPosition
	* 物体在空间中的位置
	* 
	* \param worldViewMat
	* 世界view矩阵
	* 
	* \param projectionMat
	* 世界投影矩阵
	* 
	* \param x
	* 屏幕x
	* 
	* \param y
	* 屏幕y
	* 
	* \param width
	* 屏幕宽
	* 
	* \param height
	* 屏幕高
	* 
	* \param minZ
	* 近平面
	* 
	* \param maxZ
	* 远平面
	* 
	* \param screenPosition
	* 输出的屏幕坐标
	*/
	void get_screen_position(const H3DVec3& objectPosition, const H3DMat4& worldViewMat, const H3DMat4& projectionMat, 
		unsigned int x, unsigned int y, unsigned int width, unsigned height, float minZ, float maxZ, H3DVec3& screenPosition) const;

	/**
	* \brief
	* 获得平行投影矩阵
	* 
	* \param left
	* left坐标
	* 
	* \param right
	* right坐标
	* 
	* \param bottom
	* bottom坐标
	* 
	* \param top
	* top坐标
	* 
	* \param fnear
	* 近平面距离
	* 
	* \param ffar
	* 远平面距离
	* 
	* \param out
	* 输出结果
	*/
	void get_ortho_projection_matrix(float left, float right, float bottom, float top, float fnear, float ffar, H3DMat4& out) const;

	/**
	* \brief
	* 获得窗口句柄
	*
	* \returns
	* 窗口句柄
	*/
	HWND get_window_handle() const;

	/**
	* \brief
	* 让骨骼模型预加载动作
	*
	* \param actionSequence
	* 动作序列
	*
	* \param skeletonModel
	* 骨骼模型
	*/
//	void preloadActions(const H3DActionNameSequence& actionSequence, H3DSkeletonModel *skeletonModel);

	/**
	* \brief
	* 清除已经加载的动作
	*
	*/
	void clear_loaded_actions();

	/**
	* \brief
	* 清除渲染系统
	* 
	*/
	void cleanUp();

	/**
	* \brief
	* 获得原始特效管理句柄
	*
	* \returns
	* 特效管理句柄
	*/
	ISpecialEffectManager* getIEffectManager() const;
	void setIEffectManager(ISpecialEffectManager* effectManager);

	/**
	* \brief
	* 获得原始渲染句柄
	*
	* \returns
	* 渲染句柄
	*/
	virtual H3DI::IRender* get_IRender() const ;

	virtual void setIRender(H3DI::IRender* render) ;

   /**
   * \brief
   * 设置视角摄像机
   * 
   * \param camera
   * 摄像机
   *
   * \returns
   * 老的摄像机
   */
   virtual CCameraEntity* set_render_camera(CCameraEntity* camera);

   /**
   * \brief
   * 设置视角摄像机
   * 
   * \param camera
   * 摄像机
   *
   * \returns
   * 老的摄像机
   */
   //virtual CCameraEntity* set_render_camera(const std::string& camera_name);   
   /**
   * \brief
   * 获得视角摄像机
   *
   * \returns
   * 绘制摄像机
   */
   virtual CCameraEntity* get_render_camera() const;

   /**
   * @brief
   *		渲染对象
   */
   void render();

   /**
   * @brief
   *		画灯光的线
   */
   virtual void drawLightLine(CPrePassLightEntity* light);

   	//设置线的shaderlib
   virtual void set_line_shader_ui_lib_path(const char* path);

   //绘制点
   void draw_point(const H3DVec3& pos, float radius, const H3DColor& color);
   //绘制方向光起点
   void draw_light_start_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   //绘制方向光终点
   void draw_light_end_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   //绘制连接线
   void draw_link_line(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   void draw_index(H3DI::PRIMITIVE_TYPE type, float* points, int point_count, int* indexs, int index_count, const H3DColor& color);
   H3DVec3 get_point_disN_to_point(const H3DVec3& start, const H3DVec3& dir, float dis);

   void log(const char* szString, ...);

private:


	/**
	* @brief
	* 初始化和Effect相关的成员变量
	* 创建这个函数的目的是为了不让initialize函数太长
	*
	* @return 
	*	 0 ： success
	*	 其他： 失败
	* 备注： 现在暂时只返回0或者1
	*/
	int init_effectd_variable(const std::string& effectDllFile
		, const std::string& effectManagerCreatorName
		, const std::string& effectManagerDeleteName
		, std::string* errorMsg);

	//根据角度、高度、方向、位置 颜色画一个锥形线框
	void draw_a_wimble( float angle, float lightRange, H3DQuat quat, int LineNum, H3DVec3 &pos, H3DMat4 mat, float* color, H3DI::IRender * irender );
	//根据半径、 颜色画一个球形线框
	void draw_a_sphere( float lightRange, int LineNum, H3DVec3 &pos, float* color, H3DI::IRender * irender );
	//画线
	void draw_line( float* points, int LightNum,float *color, H3DI::IRender* irender);

private:
	CGraphicsResMng*	m_res_mng;
	H3DI::IRender* m_render;
	ISpecialEffectManager* m_effect_manager;

	DeleteRenderPtr m_delete_renderer_dx;
	DeleteISpecialEffectManager  m_delete_ispecial_effect_manager;

	HMODULE m_engine_module;
	HMODULE m_effect_module;
	HMODULE m_sound_module;  //声音模块的地址

	CCameraEntity* m_render_camera;  //
	std::string	   m_shader_ui_lib_path; ///< 画2d线用的ui库的路径

	H3DColor			m_ambient_color; ///< 背景光颜色
	H3DColor			m_ambient_actor_color; ///< 人物背景光颜色

	H3DI::INewShader*	m_draw_shader;
	
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CRenderSystem);
};

#endif //_RENDER_SYSTEM_H_
