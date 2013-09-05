/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   11:03
	filename: 	D:\Code\X52\Engine_MT_Main\SDK\Include\Interface\engine_interface_debug.h
	file path:	D:\Code\X52\Engine_MT_Main\SDK\Include\Interface
	file base:	engine_interface_debug
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "engine_interface.h"
namespace H3DI
{
	//��������ṩ������Ϣ��Ϊ�˷��㴴��IRenderableProxy���������κ�����
	class IRenderable
	{
	public:
		virtual ~IRenderable()=0{}
	};

	class IRenderableProxy
	{
	public:
		virtual void Release()=0;
		virtual INewShader* GetNewShader()=0;
	};

	//���ӿ������ʵ�֣��������棬���渺��ص�
	class IDrawEventListener
	{
	public:
		virtual void Release()=0;
		virtual void OnDrawOP(IRenderable* p)=0;
		virtual void OnFrameBegin()=0;
		virtual void OnFrameEnd()=0;

	};

	class IDataStatisitcs
	{
	public:
		//virtual void Release()=0;
		virtual unsigned int GetItemCount()=0;
		virtual const char* GetItemNameByIdx(unsigned int idx)=0;
		virtual int GetItemTypeByIdx(unsigned int idx)=0; //1:int; 2:float 3:const char*
		virtual void* GetItemValueByIdx(unsigned int idx)=0;

		virtual void Register_ForwardShadingDrawEvent_Listener(IDrawEventListener* p)=0;
		virtual void UnRegister_ForwardShadingDrawEvent_Listener(IDrawEventListener* p)=0;
	};

	class IMatMacroCon
	{
	public:
		virtual int GetCount()=0;
		virtual const char* GetMacroName(int idx)=0;
		virtual const char* GetMacroValue(int idx)=0;

	};

	//��ʾһ������lod�����е�nodevar�����������ֻ��ʹ�ã�����Ҫ�ͷ�
	class IMatRSCon
	{
	public:
		enum eRS_SLOT
		{
			USE_BLEND,
			BLEND_SRC,
			BLEND_DST,
			USE_ALPHATEST,
			ALPHATEST_REF,
			USE_DEPTHTEST,
			DEPTHTEST_FUN,
			USE_DEPTHWRITE,
			USE_SCISSOR,

			eRS_End
		};
		virtual int GetRSValue(eRS_SLOT e)=0;
	};
	//��ʾһ��nodevar�����ֻ��ʹ�ã�����Ҫ�ͷ�
	class IMatNodeVar
	{
	public:
		virtual const char* GetName()=0;
		virtual const char* GetType()=0; //"int" "float" "str"
		virtual float GetValue_float()=0;
		virtual void SetValue_float(float f)=0;

		virtual int GetValue_int()=0;
		virtual void SetValue_int(int n)=0;

		virtual const char* GetValue_str()=0;
		virtual void SetValue_str(const char* str)=0;
	};
	//��ʾһ������lod�����е�nodevar�����������ֻ��ʹ�ã�����Ҫ�ͷ�
	class IMatNodeVarCon
	{
	public:
		virtual int GetCount()=0;
		virtual IMatNodeVar* GetVarByIdx(int idx)=0;
		virtual bool FindEqualVar(IMatNodeVar* p)=0;
	};

	//��ʾһ�����ʵ�һ��lod�����ֻ��ʹ�ã�����Ҫ�ͷ�
	class IMatLodData
	{
	public:
		virtual int GetEffectType()=0; //Normal=0 TwoSideAlphaBlend=3 MultiPassBlend=4

		virtual bool HasAlphaTest()=0;
		virtual bool HasTransparent()=0;
		virtual bool IsTwoSides()=0;
		virtual int GetAlphaTestRef()=0;
		virtual bool IsUseAutoLod()=0;
		virtual IMatNodeVarCon* GetNodeVars()=0;
		virtual IMatRSCon* GetRS(int shadercache)=0; //0:CS_CACHE,1:REFLECT_CACHE,3:SD_CACHE(��ö��SHADER_CACHE_TYPE
		virtual IMatMacroCon* GetMacros()=0;

		virtual H3DI::ITexture* GetDiffuseTexture()=0;
		virtual H3DI::ITexture* GetNormalMapTexture()=0;
		virtual H3DI::ITexture* GetSpecluarTexture()=0;
	};


	enum IShaderConstType
	{
		ISCT_INT,
		ISCT_TEXTURE,
		ISCT_FLOAT1,
		ISCT_FLOAT4,
		ISCT_OTHER
	};

	class IShaderConstProxy
	{
	public:
		virtual const char* GetName()=0;
		virtual const char* GetClassType()=0;
		virtual H3DI::IShaderConstType GetType() = 0;


		virtual void* GetValue()=0;
		virtual int* GetValue_int()=0;
		virtual H3DI::ITexture* GetValue_texture()=0;
		virtual float* GetValue_float1()=0;
		virtual H3DVec4* GetValue_float4()=0;
		
	};

	class IShaderProxy
	{
	public:
		virtual void Release()=0;
		virtual const char* GetFxFileName()=0;
		virtual IShaderConstProxy* GetShaderConstByName(const char*)=0;

		/// ����һ���ַ��������ַ
		virtual unsigned int GetShaderConstCount(const char** namearray, int arraysize) = 0;
	};


	class IMatProxy
	{
	public:
		virtual void Release()=0;
		virtual int GetMatLodDateNum()=0;
		virtual IMatLodData* GetLodData(int idx)=0;
		virtual bool HasBakeTexture()=0;
		virtual bool HasTransparentInDof()=0;
		virtual bool IsBeam()=0;
		virtual bool RenderInReflect()=0;
		virtual bool HasCustomShadow()=0;
		virtual bool IsReciveShadow()=0;
		virtual bool IsCastShadow()=0;

		virtual bool IsReciveFog()=0;
		virtual bool IsReciveLight()=0;
		virtual bool IsDrawAfterDof()=0;
		virtual bool IsReleaseTextureImmediately()=0;
		virtual bool IsUseVertexColor0()=0;
		virtual bool IsUseVertexColor1()=0;
		virtual bool GetLightMode()=0;
		virtual bool HasNormalMap()=0;
		virtual int  GetSortKey()=0;
		//virtual const char* GetFxFileFullName(int shadercache)=0; //3��ʾǰ����Ⱦ��shader(��ö��SHADER_CACHE_TYPE)
		virtual IShaderProxy* GetCachedShaderBytype(int shadercache)=0;//3��ʾǰ����Ⱦ��shader(��ö��SHADER_CACHE_TYPE)

	};

	class IModelProxy
	{
	public:
		virtual void Release()=0;
	};

	class IResInfo
	{
	public:
		virtual void Release()=0;
		virtual int GetCount()=0;
		virtual const char* GetResKeyName(unsigned int idx)=0;
		virtual const char* GetResFileName(unsigned int idx)=0;
		virtual int GetRef(unsigned int idx)=0;
		//����һ��ָ���Ƿ�����Դ�б���
		virtual bool FindPtr(void* ptr)=0;
	};

	struct tTexDesc
	{
		unsigned int nWidth;//��
		unsigned int nHeight;//��
		bool bDDS;//�Ƿ�DDS
		bool bCubeMap;//�Ƿ�CubeMap
		int nMipmap;//MipMap�ȼ�
	};

	class ITexProxy
	{
	public:
		virtual void Release()=0;
		virtual void GetTexDesc(tTexDesc* desc)=0;
	};

	class IGlobalEnvironment
	{
	public:
		virtual IMatProxy* CreateIMatProxyFromFile(const char* filename,const char* ball)=0;
		virtual IMatProxy* CreateIMatProxyFromINewShader(H3DI::INewShader* pshader)=0;
		virtual IRenderableProxy* CreateRenderableProxy(H3DI::IRenderable* prenderable)=0;
		virtual ITexProxy* CreateITexProxyFromITexture(H3DI::ITexture* ptex)=0;

		//0:cast shader
		//1:reflection
		//3:lpp
		//4:forward shading
		virtual unsigned int GetLastGenFxTimeStamp(int pipeType)=0;

		//�õ���������������ɼ��ص���ͼ��Դ
		virtual IResInfo* GetAllLoadedTextureResInfo()=0;

		virtual IDataStatisitcs* GetDataStatisitcs()=0;
		virtual bool IsPhyXClothOpen()=0;
		virtual void SetPhyXClothOpen(bool b)=0;
	};

	class IProxyFactory
	{
	public:
		virtual void Release()=0;
		virtual IGlobalEnvironment* GetGlobalEnv()=0;

	};

	IProxyFactory* CreateIProxyFactory();
}