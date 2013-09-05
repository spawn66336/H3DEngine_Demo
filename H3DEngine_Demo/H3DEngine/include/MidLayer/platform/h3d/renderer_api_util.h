/*********************************************************************
	Created:	2003/11/24
	File name:	UIWndManager.h
	Author:		maple
	
	Copyright (C) 2003 - 2005 H3D, All Rights Reserved.
	
	Purpose:	
*********************************************************************/

#ifndef __H3D_CLIENT_RENDERER_API_UTIL_H__
#define __H3D_CLIENT_RENDERER_API_UTIL_H__

#include <client/predefined.h>
#include <client/i_timer.h>
#include <client/i_config.h>

#include "RenderCore.h"

#ifdef RENDERCORE_NEW

#include "RenderStateManager.h"
#include <common_lib/mathlib/dMathHeader.h>
#include <client_ui/i_ui.h>

#else

#include <opengl/gl.h>
#include <opengl/glu.h>

#endif

namespace H3DI
{
	class IRender;
}

BEGIN_H3D_CLIENT

extern void safe_check_assign_boundary(void* pos, void* beg, void* end);
extern void safe_check_memcpy_boundary(void* des, void* org, int countinchar, void* desbeg, void* desend, void* orgbeg, void* orgend);

// ԭ��ָ��ָ�������ڴ�ʱ���Ľ��İ�ȫ��ֵ����
// [ )
/* �����÷�
char* buf = new char[1024];
for(int i = 0; i < 1024; ++i) // ԭ��������
	*buf = char('a');

// ���ڵ�����
char* beg = buf;
char* end = buf + 1024;
for(int i = 0; i < 1024; ++i) 
	safe_assign_value_to_ptr(buf, beg, end, char('a'));
*/

// �޸�ָ����ָ���λ������Ӧ����ֵ��̽�������е��ڴ�������ᳬ�������ķ�Χ������ҿ�
template <class T>
inline void safe_assign_value_to_ptr(T* pos, T* beg, T* end, const T& value)
{
#ifndef _FINAL_RELEASE
	safe_check_assign_boundary(pos, beg, end);
#endif
	*pos = value;
}

// ԭ��ָ��ָ�������ڴ�ʱ���Ľ��İ�ȫ�ڴ濽������
// [ )
/* �����÷�
char* org = new char[1024];
...
char* des = new char[1024];

// ԭ��������
memcpy(des, org, 1024);

// ���ڵ�����
char* orgbeg = org;
char* orgend = org + 1024;
char* desbeg = des;
char* desend = des + 1024;
safe_memcpy_to_ptr(des, org, 1024, desbeg, desend, orgbeg, orgend);
*/

// ��ȫ�Ľ����ڴ渴��
inline void safe_memcpy_to_ptr(void* des, void* org, int countinchar, void* desbeg, void* desend, void* orgbeg, void* orgend)
{
#ifndef _FINAL_RELEASE
	// fixme: ����Ӧ�ü�һ���жϣ���Ҫcountinchar�ĵ�λΪchar
	safe_check_memcpy_boundary(des, org, countinchar, desbeg, desend, orgbeg, orgend);
#endif
	memcpy(des, org, countinchar * sizeof(char));
}

class MemCheck
{
public:
	MemCheck(float k)
	{
#ifndef _FINAL_RELEASE
		int kB = k * 1024;
		m_buf = new char[kB];
#endif
	}

	~MemCheck()
	{
#ifndef _FINAL_RELEASE
		delete []m_buf;
#endif	
	}

protected:

#ifndef _FINAL_RELEASE
	char* m_buf;
#endif	
};

class MemCheckChain
{
public:
	MemCheckChain(int keeps)
#ifndef _FINAL_RELEASE
		: m_keeps(keeps)
#endif
	{
	}

	~MemCheckChain()
	{
#ifndef _FINAL_RELEASE
		ensureKeeps(0);
#endif
	}

#ifndef _FINAL_RELEASE
	void check(float k, int count);
#else
	void check(float k, int count){}
#endif

	void ensureKeeps(int keeps)
	{
#ifndef _FINAL_RELEASE
		while(m_check_buf.size() > keeps)
		{
			delete m_check_buf.front();
			m_check_buf.pop_front();
		}
#endif	
	}


protected:
	int m_keeps;
	std::list<MemCheck*> m_check_buf;
};
extern void faild(const _tchar* content = 0);
template<typename T>
class ArrayWarpper
{
public:
	ArrayWarpper()
	{
		m_array = 0;
		m_len = 0;
	}
	//Ϊ�ö������һ�γ�len�����顣len < 0��assert��
	ArrayWarpper(int len)
	{
		m_array = 0;
		m_len = 0;
		create(len);
	}
	//���value_warpper��������ݵ�������
	ArrayWarpper(const ArrayWarpper& value_warpper)
	{
		m_array = 0;
		m_len = 0;
		create(value_warpper);
	}
	~ArrayWarpper()
	{
		clear();
	}
public:
	//Ϊ�ö������һ�γ�len�����顣len < 0��assert��
	bool create(int len)
	{
		if (len < 0)
		{
			faild(_T("ArrayWarpper createA len < 0"));
			return false;
		}
		clear();
		m_len = len;
		if (len > 0)
		{
			m_array = new T[len];
		}
		return true;
	}
	//Ϊ�ö������һ�γ�len�����飬����valueΪ������ÿ���ڵ㸳��ֵ��len < 0��assert��
	bool create(int len, const T& value)
	{
		if (len < 0)
		{
			faild(_T("ArrayWarpper createB len < 0"));
			return false;
		}
		clear();
		m_len = len;
		if (len > 0)
		{
			m_array = new T[len];
			for (int i = 0; i < len; i++)
			{
				m_array[i] = value;
			}
		}
		return true;
	}
	//Ϊ�ö������һ�γ�len�����飬����value_array��ÿ���ڵ�ֵΪ��Ӧ������ÿ���ڵ㸳��ֵ��len < 0��assert��
	//value_array==0ʱ���ú�����ͬ��bool create(int len)
	//value_array!=0ʱ,�ⲿ���뱣֤value_array�������ٰ���len���ڵ�
	bool create(int len, const T* value_array)
	{
		if (len < 0)
		{
			faild(_T("ArrayWarpper createC len < 0"));
			return false;
		}
		clear();
		m_len = len;
		if (len > 0)
		{
			m_array = new T[len];
			if (value_array)
			{
				for (int i = 0; i < len; i++)
				{
					m_array[i] = value_array[i];
				}
			}
		}
		return true;
	}
	//���value_warpper��������ݵ�������
	bool create(const ArrayWarpper& value_warpper)
	{
		if (value_warpper.m_len < 0)
		{
			faild(_T("ArrayWarpper createD len < 0"));
			return false;
		}
		clear();
		m_len = value_warpper.m_len;
		if (value_warpper.m_len > 0)
		{			
			m_array = new T[value_warpper.m_len];
			for (int i = 0; i < value_warpper.m_len; i++)
			{
				m_array[i] = value_warpper.m_array[i];
			}
		}
	}
	void clear()
	{
		if (m_array)
		{
			delete[] m_array;
			m_array = 0;
		}
		m_len = 0;
	}
	bool empty()
	{
		return m_len <= 0;
	}
	const T* getArray() const
	{
		return m_array;
	}
	const int getSize() const
	{
		return m_len;
	}
public:
	const ArrayWarpper& operator=(const ArrayWarpper& value_warpper)
	{
		create(value_warpper);
		return *this;
	}
	//ͨ������ȡ�����Ӧ�ڵ����ݣ�����<0����>=���鳤�Ȼ�assert
	T& operator[](int value_index)
	{
		if (value_index < 0 || value_index >= m_len)
			faild(_T("ArrayWarpper createD array = 0"));
		return m_array[value_index];
	}
	//ͨ������ȡ�����Ӧ�ڵ����ݣ�����<0����>=���鳤�Ȼ�assert
	const T& operator[](int value_index) const
	{
		if (value_index < 0 || value_index >= m_len)
			faild(_T("ArrayWarpper createD array = 0"));
		return m_array[value_index];
	}
protected:
	int   m_len;
	T*    m_array;
};
#ifdef RENDERCORE_NEW

// ��ʱʱ�εļ���
class TimeSpanCheck
{
public:
	TimeSpanCheck(const _tchar* str);
	void doCheck(int span);
protected:
	I_Timer::MS_TYPE m_beg_t;
	_tstring m_info;
};

class Platform_Info_Log
{
	_tstring m_name;
	size_t m_mem_used;
	size_t m_video_mem_used;

	//static ConfigValue::ValueSet* s_info_retrive;
	static bool s_info_retrive;

public:
	Platform_Info_Log(const _tchar* name);
	~Platform_Info_Log();
};

class Time_Info_Log
{
	_tstring m_name;
	I_Timer::MS_TYPE m_org_time;

public:
	Time_Info_Log(const _tchar* name);
	~Time_Info_Log();
};


class FrameSpanAction
{
	static int s_span_tiny;
	static int s_span_big;

public:
	FrameSpanAction(const _tchar* info);

	struct FrameSpanInfo
	{
		I_Timer::MS_TYPE m_beg_t;
		_tstring m_name;
		int m_span;
	};

	static std::list<FrameSpanInfo> s_big_frame_span_info;
	static void resetSpan();
};
#define clFrameSpan(exp) {H3D_CLIENT::FrameSpanAction(_T(#exp));}
#define clFrameResetSpan(){H3D_CLIENT::FrameSpanAction::resetSpan();}
class HWR
{
	static bool s_is_dx9;
public:
	static H3DI::IRender* s_renderer;
	static int s_znear, s_zfar;

	static inline bool validProjectionPoint(float p)
	{
		return p > 0 && p < 1;
	}
	static inline void setDx9(bool isdx9)
	{
		s_is_dx9 = isdx9;
	}
	static inline bool dx9()
	{
		return s_is_dx9;
	}
	static inline void load_viewmatrix_identity()
	{
		H3DMat4 mat;
		mat.Identity();
		s_renderer->SetViewMatrix(mat);
	}
	static inline void load_projection_identity()
	{
		H3DMat4 mat;
		mat.Identity();
		s_renderer->SetPerspective(mat);
	}
	static inline void set_lighting(bool isset)
	{
	}
	static inline void set_texture2d(bool isset)
	{
	}
	static inline void set_scissortest(bool isset)
	{
		//s_renderer->GetRenderStateManager()->SetRenderState(H3DI::RS_SCISSORTESTENABLE, isset ? 1 : 0);
	}
	static inline void set_blend(bool isset)
	{
	}
	static inline void set_depthtest(bool isset)
	{
	}	
	static inline void render_cursor(int x, int y, int w, int h)
	{
	}
	static inline void set_color(float r, float g, float b)
	{
	}
	static inline void set_color(float r, float g, float b, float a)
	{
	}
	static inline void set_color(float* c)
	{
	}
	static inline void set_scissor(int x, int y, int w, int h)
	{
		s_renderer->GetRenderStateManager()->SetScissorRect(x, x + w, y, y + h);
	}
	static inline void set_ortho(int left, int right, int bottom, int top, int znear, int zfar)
	{
		H3DMat4 mat;
		s_renderer->GetOrthoProjectionMatrix(left, right, bottom, top, znear, zfar, mat);
		s_renderer->SetPerspective(mat);

		s_znear = znear;
		s_zfar = zfar;
	}
};

#else

class HWR
{
public:
	static int s_znear, s_zfar;
	static H3DI::IRender* s_renderer;
	static inline bool validProjectionPoint(float p)
	{
		return p > - 1 && p < 1;
	}
	static inline void push_view_matrix()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	}
	static inline void pop_view_matrix()
	{
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	static inline void translate_view_matrix(float x, float y, float z)
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(x, y, z);
	}
	static inline void rotate_view_matrix(float value, float x, float y, float z)
	{
		glMatrixMode(GL_MODELVIEW);
		glRotatef(value, x, y, z);
	}
	static inline void load_viewmatrix_identity()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	static inline void push_projection_matrix()
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
	}
	static inline void pop_projection_matrix()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	static inline void load_projection_identity()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	static inline void set_lighting(bool isset)
	{
		if(isset)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
	}
	static inline void set_texture2d(bool isset)
	{
		if(isset)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
	static inline void set_scissortest(bool isset)
	{
		if(isset)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
	}
	static inline void set_blend(bool isset)
	{
		if(isset)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
	static inline void set_depthtest(bool isset)
	{
		if(isset)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}	
	static inline void render_cursor(int x, int y, int w, int h)
	{
		glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x, y+h);
		glVertex2i(x+w, y);
		glVertex2i(x+w, y+h);
		glEnd();
	}
	static inline void set_color(float r, float g, float b)
	{
		glColor3f(r, g, b);
	}
	static inline void set_color(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}
	static inline void set_color(float* c)
	{
		glColor4f(c[0], c[1], c[2], c[3]);
	}
	static inline void set_scissor(int x, int y, int w, int h)
	{
		glScissor(x, y, w, h);
	}
	static inline void set_ortho(int left, int right, int bottom, int top, int znear, int zfar)
	{
		glOrtho(left, right, bottom, top, znear, zfar);
		s_znear = znear;
		s_zfar = zfar;
	}
};

#endif

// ���������Լ���Ӧ�����,����ö�Ӧ�����������Ϣ
class PBufferShoot_Util
{
public:
	PBufferShoot_Util(H3DI::IRender* renderer, I_PBuffer* pbuffer);

	// ��Pbuffer����Ϊ��ǰ�Ļ���Ŀ��
	void setupEnv(float clearc[]);
	// �ָ�ԭ���Ļ���Ŀ��
	void clearEnv();
	// ������άͶӰ���
	void setCamera(float fovy, float aspect, 
		float n, float f, 
		float eyepos[], float eyedes[], float eyeup[], unsigned short vp[]);
	// ����ƽ��ͶӰ���
	void setCamera(float n, float f, unsigned short vp[]);
	// ȡ�����õ����
	void desetCamera(unsigned short oldvw, unsigned short oldvh);
	// ʹ�ø���shader����
	void renderMask(int ow, int oh, int mx, int my, int mw, int mh, H3DI::IShader* ms);

protected:
	H3DI::IRender* m_renderer;
	I_PBuffer* m_pbuffer;
};
/*
�������ɣ��ɲ߻�ʹ�ù������ɸߴα��������ߣ����ߺ�����������Ƶ㣬���Ƶ����Ϣ������xml�С�
*/

typedef struct
{
	float x,y;
} BPOINT;

//�������ͷָ�룬���Ƶ�ĸ��������е�ĸ���-1�����������ָ�룬 Ҫ��ĵ������buf�ĳ��ȣ�
//������pPoint ���ߵ����п��Ƶ�
//������num ���Ƶ�ĸ���
//������buf�����ϵĵ��ָ�룬�ڴ����ⲿ�������
//������len ��Ҫ���ٸ��㣬�����������buf�ĳ���

//����ֵ��0��ʾʧ��
//�ɹ��Ļ�����buf��ʵ�����ĳ���
//  v0 �����ĳ��ٶȴ�С����ΧΪ0��1 ֮��ĸ��㣬����1ʱ�켣Ϊ���٣�����1��Ϊ���ٹ켣������Ϊ0ʱ����Ч�������ԣ�
inline int CalBezier(BPOINT* pPoint, int num, BPOINT *buf, int len, float v0 = 1.0f)
{
	if (v0 <= -0.000001 || v0 > 1.0f)//"v0��ֵ��Ҫ��0��1֮��" 
	{
		faild(L"CalBezier v0 <= -0.000001 || v0 > 1.0f");
		return 0;
	}
	num -= 1;
	vector<BPOINT> ptVec;
	vector<float>  lineSingle;
	int m_pdis = 3;
	const int STACK_SIZE = 2<<13;
	BPOINT	polygonstack[STACK_SIZE];
	int	 stacktop = 0;
	if (num <= 0 || buf == NULL)
	{
		return 0;
	}
	BPOINT* oldpoint;
	BPOINT* newpoint;

	int  i;
	int  j;

	if (num <= 0) return 0; 

	for (i=0; i<=num; i++)
	{
		if (stacktop >= STACK_SIZE) return 0;
		polygonstack[stacktop++] = pPoint[i] ;
	}
	while (stacktop > 0)
	{
		newpoint = &(polygonstack[stacktop]);
		oldpoint = newpoint - (num + 1);


		stacktop += (num + 1);
		if (stacktop >= STACK_SIZE) return 0;

		*newpoint = *oldpoint;

		for (i=1; i<=num; i++)
		{
			for (j=0; j<=num-i; j++)
			{
				oldpoint[j].x=(oldpoint[j].x+oldpoint[j+1].x)/2;
				oldpoint[j].y=(oldpoint[j].y+oldpoint[j+1].y)/2;
			}
			newpoint[i]=oldpoint[0];
		}
		if (abs(oldpoint[0].x-oldpoint[num].x)+
			abs(oldpoint[0].y-oldpoint[num].y) <= m_pdis)
		{
			ptVec.push_back(*oldpoint);
			stacktop -= 2*(num + 1);
		}
	}

	float line = 0;
	const float ZERO = 0;
	lineSingle.push_back(ZERO);

	for ( vector<BPOINT>::iterator i = ptVec.begin()+1 ; i!= ptVec.end() ; i++)
	{
		line  +=   sqrt((i->x - (i - 1)->x) * (i->x - (i - 1)->x) +
			(i->y - (i - 1)->y) * (i->y - (i - 1)->y));

		lineSingle.push_back(line);
	}
	v0=v0*line / len;
	float a = 2*(line - v0*len)/static_cast<float>(len)/ static_cast<float>(len);

	for (int i = 0; i < len ;  i++)
	{
		float dis = static_cast<float>(v0*(i+1) + 0.5*a*(i+1)*(i+1));
		vector<float> ::iterator k=lineSingle.begin();
		vector<BPOINT>::iterator l = ptVec.begin();
		for( ; l!=ptVec.end()&&k!= lineSingle.end(); l++,k++)
		{
			if (dis <= *k )
			{
				*(buf+i)=*l;
				break;
			}
		}
	}

	line = 0;
	ptVec.clear();
	lineSingle.clear();
	return len;
}

END_H3D_CLIENT

#endif // __H3D_CLIENT_RENDERER_API_UTIL_H__