/**
* @file render_system.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.26
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
	* ��ʼ��
	* 
	* \returns
	* ����ɹ�����1, ���򷵻�0
	*/
	virtual bool initialize(H3DI::IRender* irender);

	/**
	* \brief
	* ���ø������Թ���
	*
	* \param afmode
	* �������Թ��˲���
	*/
	void set_af_mode(H3DI::EAFMode afMode);

	/**
	* \brief
	* ��ø������Թ���
	*
	* \returns
	* �������Թ��˲���
	*/
	H3DI::EAFMode get_af_mode() const;

	/**
	* \brief
	* ���÷����
	*
	* \param afmode
	* ����ݲ���
	*/
	void set_aa_mode(H3DI::EAAMode aaMode);

	/**
	* \brief
	* ��÷����
	*
	* \returns
	* ����ݲ���
	*/
	H3DI::EAAMode set_aa_mode() const;

	/**
	* \brief
	* ������Ļ�����־λ
	*
	* �������������²����Ľ��(ͨ��|���), 
	* CLEAR_BUF_COLOR = 1
	* CLEAR_BUF_DEPTH = 2
	* CLEAR_BUF_STENCIL = 4
	*
	* \param flags
	* �����־
	*/
	void set_clear_flags(unsigned int flags);

	/**
	* \brief
	* �����Ļ�����־
	*
	* \returns
	* �����־
	*/
	unsigned int get_clear_flags() const;


	/**
	* \brief
	* �����Ƿ�ʹ���������б�
	*
	* \param use
	* �Ƿ�ʹ���������б�
	*/
	void set_use_triangle_list(bool use);

	/**
	* \brief
	* ����Ƿ�ʹ���������б�
	*
	* \returns
	* �Ƿ�ʹ���������б�
	*/
	bool get_use_triangle_list() const;

	/**
	* \brief
	* �����Ƿ�ʹ�ô�ֱͬ��
	*
	* \param veticalSync
	* �Ƿ�ʹ�ô�ֱͬ��
	*/
	void set_vetical_sync(bool veticalSync);

	/**
	* \brief
	* ��ô�ֱͬ����־
	*
	* \returns
	* �Ƿ�ʹ�ô�ֱͬ��
	*/
	bool get_vetical_sync() const;

	/**
	* \brief
	* ��ʼ��Ⱦ
	*
	* ÿ����Ⱦ֮ǰ��Ҫ�ȵ����������
	*
	*/
	void begin_render();

	/**
	* \brief
	* ������Ⱦ
	*
	* ÿ����Ⱦ������Ҫ�ȵ����������
	*
	*/
	void end_render();

	/**
	* \brief
	* ������Ļ��ָ���ļ�
	*
	* \param fileName
	* ͼƬ�ļ���������
	*/
	void save_screen_to_file(const std::string& fileName);

	/**
	* \brief
	* ���֧�ֵ����afֵ
	*
	* \returns
	* afֵ
	*/
	H3DI::EAFMode get_supported_af_mode();

	/**
	* \brief
	* ���֧�ֵ����aaֵ
	*
	* \returns
	* aaֵ
	*/
	H3DI::EAAMode get_supported_aa_mode();

	/**
	* \brief
	* ���ô���ģʽ, ������ȫ��, ������������
	*
	* \param windowMode
	* ����ģʽ
	*/
	void set_window_mode(WINDOW_MODE windowMode);

	/**
	* \brief
	* ����3d�ռ���λ���ڵ�����Ļ����ϵ�µ�λ��
	* 
	* \param objectPosition
	* �����ڿռ��е�λ��
	* 
	* \param worldViewMat
	* ����view����
	* 
	* \param projectionMat
	* ����ͶӰ����
	* 
	* \param x
	* ��Ļx
	* 
	* \param y
	* ��Ļy
	* 
	* \param width
	* ��Ļ��
	* 
	* \param height
	* ��Ļ��
	* 
	* \param minZ
	* ��ƽ��
	* 
	* \param maxZ
	* Զƽ��
	* 
	* \param screenPosition
	* �������Ļ����
	*/
	void get_screen_position(const H3DVec3& objectPosition, const H3DMat4& worldViewMat, const H3DMat4& projectionMat, 
		unsigned int x, unsigned int y, unsigned int width, unsigned height, float minZ, float maxZ, H3DVec3& screenPosition) const;

	/**
	* \brief
	* ���ƽ��ͶӰ����
	* 
	* \param left
	* left����
	* 
	* \param right
	* right����
	* 
	* \param bottom
	* bottom����
	* 
	* \param top
	* top����
	* 
	* \param fnear
	* ��ƽ�����
	* 
	* \param ffar
	* Զƽ�����
	* 
	* \param out
	* ������
	*/
	void get_ortho_projection_matrix(float left, float right, float bottom, float top, float fnear, float ffar, H3DMat4& out) const;

	/**
	* \brief
	* ��ô��ھ��
	*
	* \returns
	* ���ھ��
	*/
	HWND get_window_handle() const;

	/**
	* \brief
	* �ù���ģ��Ԥ���ض���
	*
	* \param actionSequence
	* ��������
	*
	* \param skeletonModel
	* ����ģ��
	*/
//	void preloadActions(const H3DActionNameSequence& actionSequence, H3DSkeletonModel *skeletonModel);

	/**
	* \brief
	* ����Ѿ����صĶ���
	*
	*/
	void clear_loaded_actions();

	/**
	* \brief
	* �����Ⱦϵͳ
	* 
	*/
	void cleanUp();

	/**
	* \brief
	* ���ԭʼ��Ч������
	*
	* \returns
	* ��Ч������
	*/
	ISpecialEffectManager* getIEffectManager() const;
	void setIEffectManager(ISpecialEffectManager* effectManager);

	/**
	* \brief
	* ���ԭʼ��Ⱦ���
	*
	* \returns
	* ��Ⱦ���
	*/
	virtual H3DI::IRender* get_IRender() const ;

	virtual void setIRender(H3DI::IRender* render) ;

   /**
   * \brief
   * �����ӽ������
   * 
   * \param camera
   * �����
   *
   * \returns
   * �ϵ������
   */
   virtual CCameraEntity* set_render_camera(CCameraEntity* camera);

   /**
   * \brief
   * �����ӽ������
   * 
   * \param camera
   * �����
   *
   * \returns
   * �ϵ������
   */
   //virtual CCameraEntity* set_render_camera(const std::string& camera_name);   
   /**
   * \brief
   * ����ӽ������
   *
   * \returns
   * ���������
   */
   virtual CCameraEntity* get_render_camera() const;

   /**
   * @brief
   *		��Ⱦ����
   */
   void render();

   /**
   * @brief
   *		���ƹ����
   */
   virtual void drawLightLine(CPrePassLightEntity* light);

   	//�����ߵ�shaderlib
   virtual void set_line_shader_ui_lib_path(const char* path);

   //���Ƶ�
   void draw_point(const H3DVec3& pos, float radius, const H3DColor& color);
   //���Ʒ�������
   void draw_light_start_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   //���Ʒ�����յ�
   void draw_light_end_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   //����������
   void draw_link_line(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color);
   void draw_index(H3DI::PRIMITIVE_TYPE type, float* points, int point_count, int* indexs, int index_count, const H3DColor& color);
   H3DVec3 get_point_disN_to_point(const H3DVec3& start, const H3DVec3& dir, float dis);

   void log(const char* szString, ...);

private:


	/**
	* @brief
	* ��ʼ����Effect��صĳ�Ա����
	* �������������Ŀ����Ϊ�˲���initialize����̫��
	*
	* @return 
	*	 0 �� success
	*	 ������ ʧ��
	* ��ע�� ������ʱֻ����0����1
	*/
	int init_effectd_variable(const std::string& effectDllFile
		, const std::string& effectManagerCreatorName
		, const std::string& effectManagerDeleteName
		, std::string* errorMsg);

	//���ݽǶȡ��߶ȡ�����λ�� ��ɫ��һ��׶���߿�
	void draw_a_wimble( float angle, float lightRange, H3DQuat quat, int LineNum, H3DVec3 &pos, H3DMat4 mat, float* color, H3DI::IRender * irender );
	//���ݰ뾶�� ��ɫ��һ�������߿�
	void draw_a_sphere( float lightRange, int LineNum, H3DVec3 &pos, float* color, H3DI::IRender * irender );
	//����
	void draw_line( float* points, int LightNum,float *color, H3DI::IRender* irender);

private:
	CGraphicsResMng*	m_res_mng;
	H3DI::IRender* m_render;
	ISpecialEffectManager* m_effect_manager;

	DeleteRenderPtr m_delete_renderer_dx;
	DeleteISpecialEffectManager  m_delete_ispecial_effect_manager;

	HMODULE m_engine_module;
	HMODULE m_effect_module;
	HMODULE m_sound_module;  //����ģ��ĵ�ַ

	CCameraEntity* m_render_camera;  //
	std::string	   m_shader_ui_lib_path; ///< ��2d���õ�ui���·��

	H3DColor			m_ambient_color; ///< ��������ɫ
	H3DColor			m_ambient_actor_color; ///< ���ﱳ������ɫ

	H3DI::INewShader*	m_draw_shader;
	
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CRenderSystem);
};

#endif //_RENDER_SYSTEM_H_
