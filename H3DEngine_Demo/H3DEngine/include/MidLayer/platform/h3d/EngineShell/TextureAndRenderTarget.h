#ifndef TEXTURE
#define TEXTURE

#include "instance_pool.h"
#include "../../../platform/platform_shared/biboframe/biboplatform.h"

namespace H3DI
{
	class CTexture:public ITexture 
	{
	protected:
		unsigned char* m_buf;
	public:
		unsigned int m_ref;
		CTexture(int w,int h):m_buf(0), m_ref(1){ int bufsize = w*h*4; m_buf = new unsigned char[bufsize];}
		CTexture():m_buf(0), m_ref(1){}

		virtual void setWH(int w,int h) 
		{
			int bufsize = w*h*4; 
			if(!m_buf)
				m_buf = new unsigned char[bufsize];
		}

		~CTexture(){if(m_buf) delete[] m_buf;}
		//OBJECT
		virtual void					Release()
		{
			if(m_ref <= 1)
				EnginePool::m_texture_pool->releaseObj(this);
		};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};

		//material
		virtual const char*				GetName(){return 0;};
		virtual MATERIAL_TYPE			GetType(){return MATERIAL_TEXTURE;};
	//ITexture
		virtual unsigned int			GetSize(){ return 0; };
		virtual unsigned int			GetWidth(){ return 0; };
		virtual unsigned int			GetHeight(){ return 0; };
		virtual TEX_TYPE			GetTexType(){return TEXTURE_1D;  };
		virtual unsigned int			GetHandle(){ return 0; };
		virtual unsigned int			GetBit(){ return 0; };

		//!lock贴图,用于用户自己填充内容
		virtual void*                  Lock(unsigned int level, unsigned int& pitch) {return m_buf;};
		virtual void                    Unlock(void) {};
		virtual void					AddRef(){ ++m_ref; };
		virtual void					DelRef(){ alwaysAssert(m_ref > 0); --m_ref; };
		//!将贴图保存成文件,PNG格式
		virtual void                    SaveToFile(const char* )const {};
		//!return HDC ,2008-3-24
		virtual void*					GetDC(){ return 0; };
		virtual void					ReleaseDC(void* hDC){  };	

	};

	class CRenderTarget:public IRenderTarget 
	{
		int m_w, m_h;
		unsigned int m_ref;
	public:
		CRenderTarget(int w, int h):m_ref(1)
		{
			m_w = w;
			m_h = h;
		}

		CRenderTarget():m_ref(1)
		{
			m_w = -1;
			m_h = -1;
		}

		void setWH(int w, int h)
		{
			m_w = w;
			m_h = h;
		}

		virtual void					Release()
		{
			if(m_ref <= 1)
				EnginePool::m_rendertarget_pool->releaseObj(this);
		};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};

		//material
		virtual const char*				GetName(){return 0;};
		virtual MATERIAL_TYPE			GetType(){return MATERIAL_TEXTURE;};
		//ITexture
		virtual unsigned int			GetSize(){ return 0; };
		virtual unsigned int			GetWidth(){ return m_w; };
		virtual unsigned int			GetHeight(){ return m_h; };
		virtual TEX_TYPE			GetTexType(){return TEXTURE_1D;  };
		virtual unsigned int			GetHandle(){ return 0; };
		virtual unsigned int			GetBit(){ return 0; };

		//!lock贴图,用于用户自己填充内容
		virtual void*                  Lock(unsigned int level, unsigned int& pitch) {return 0;};
		virtual void                    Unlock(void) {};
		virtual void					AddRef(){ ++m_ref; };
		virtual void					DelRef(){ alwaysAssert(m_ref > 0); --m_ref; };
		//!将贴图保存成文件,PNG格式
		virtual void                    SaveToFile(const char* )const {};
		//!return HDC ,2008-3-24
		virtual void*					GetDC(){ return 0; };
		virtual void					ReleaseDC(void* hDC){  };	
	//RENDER TARGET
		virtual void Activate(unsigned int channel) {};
		virtual void Deactivate(void) {};
	};
}
#endif