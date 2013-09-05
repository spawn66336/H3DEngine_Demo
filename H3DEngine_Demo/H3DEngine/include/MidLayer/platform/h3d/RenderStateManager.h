#ifndef _RENDER_STATE_MANAGER_H_
#define _RENDER_STATE_MANAGER_H_

namespace H3DI
{
	enum RENDERSTATETYPE
	{
		RS_ZENABLE = 0,
		RS_ZWRITEENABLE,
		RS_ALPHATESTENABLE,
		RS_ALPHABLENDENABLE,
		RS_STENCILENABLE,
		RS_CLIPPLANEENABLE,
		RS_COLORWRITEENABLE,
		RS_SCISSORTESTENABLE,
		RS_END
	};

	class IRenderStateManager
	{
	public:
		virtual void SetRenderState(RENDERSTATETYPE type, unsigned int value)const = 0;
		virtual unsigned int GetRenderState(RENDERSTATETYPE type)const = 0;
		virtual void SetScissorRect(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)const = 0;

	protected:
		virtual ~IRenderStateManager(){}
	};
}
#endif