#ifndef ENGINESHELL
#define ENGINESHELL
// C����ʱͷ�ļ�
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>

// STL����
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>
#include <set>
#include <sstream>

#include "../../../platform/platform_shared/biboframe/biboplatform.h"
#include "../../../platform/platform_shared/biboframe/BiboAssert.h"

#include "../../../common_lib/mathlib/dMathHeader.h"
#include "../../../common_lib/mathlib/math/Vector.h"
#include "../engine_interface.h"
#include "SpecialEffectShell.h"
#include "../../platform_shared\test_platform\include\test_platform\mock.h"
#include "../RenderStateManager.h"
#include "LevelShell.h"
#include "TextureAndRenderTarget.h"
#include "ModelShell.h"

#include "instance_pool.h"

namespace H3DI
{
	class CObject:public IObject
	{
	public:
		//OBJECT
		virtual void					Release(){delete this;};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};

		//	virtual OBJ_TYPE				GetType()=0;
	};
	class CVB: public IVB
	{
	public:
		CVB(){ m_buf = NULL;};
		virtual void					Release(){delete[] m_buf;delete this;};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};
		//////
		virtual ~CVB(){};
		virtual char*					GetVertexBuffer(){return 0;};
		virtual char*					GetIndexBuffer(){return 0;};
		virtual void					UnLock(unsigned int nType){};

		virtual char*					Lock(unsigned int nType, unsigned int fmt, unsigned int stride){return m_buf;};
		virtual bool					CreateBuffer(unsigned int nVBufSize, unsigned int indexCount,
			unsigned int nUsage, unsigned int nElemUsage, char* pVBuf, char* pEBuf){
				m_buf = new char[nVBufSize];
				return true;
		};

		virtual unsigned int			GetMaxSize(){return 0;};
	protected:
		char* m_buf;
	};


	class CNewShader:public INewShader
	{
	public:
		CNewShader()
		{
			m_ref = 1;
		}

		//OBJECT
		virtual void Release()
		{
			if(m_ref <= 1)
				EnginePool::m_shader_pool->releaseObj(this);
		};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};

		//material
		virtual const char*				GetName(){return 0;};
		virtual MATERIAL_TYPE			GetType(){return MATERIAL_NEW_SHADER;};
		//shader
		///�õ�pass����
		virtual unsigned int GetPassNumber(){return 0;};

		///�õ�ĳ��������ֵ,���������ͣ�����ֵΪ1��ʾ�ɹ� 0��ʾʧ��
		virtual int GetNodeVarValue_Float(const char* varName,float& res){return 0;};
		virtual int GetNodeVarValue_Int(const char* varName,int& res){return 0;};
		virtual int GetNodeVarValue_Float4(const char* varName,float& f1,float& f2,float& f3,float& f4){return 0;};
		virtual int GetNodeVarValue_Str(const char* varName,const char*& res){return 0;}; //resΪ�ⲿ�����һ��ָ�룬�����ڲ����޸����ָ���ֵ
		virtual int GetNodeVarValue_Matrix44(const char* varName,float* mat){return 0;}; //matΪ����16��Ԫ�ص�����,mat�Ŀռ���ʹ���߷���

		virtual int SetNodeVarValue_Float(const char* varName,float res){return 0;};
		virtual int SetNodeVarValue_Int(const char* varName,int res){return 0;};
		virtual int SetNodeVarValue_Float4(const char* varName,float f1,float f2,float f3,float f4){return 0;};
		virtual int SetNodeVarValue_Str(const char* varName,const char* res){return 0;};
		virtual int SetNodeVarValue_Matrix44(const char* varName,float* mat){return 0;}; //matΪ����16��Ԫ�ص�����,mat�Ŀռ���ʹ���߷���

		virtual void GetMaterialInfo(MaterialInfo* mtlInfo) {};

		///������ͼ��Shader,����1��ʾ�ɹ���0��ʾʧ��
		virtual int SetTexture(const char* varName,H3DI::ITexture* pTex){return 1;};

		/// �õ��˲���ʹ�õ�ITexture��0��ʾû��
		virtual H3DI::ITexture* GetTexture(const char* varName) {return 0;};

		///����һ���������е��޸ģ��ָ���ʼֵ
		virtual int ResetVar(const char* varName){return 0;};
		///�õ�һ��shader���еı�������
		virtual int GetAllNodeVarNumber(){return 0;};
		///�õ�һ���������ֵ������ַ���
		virtual const char* GetNodeVarType(const char* varName){return 0;};
		///�õ���������
		virtual const char* GetMaterialName(){return 0;};


		///���������������ýӿڶԸò��ʵ����еģ��ڱ༭����ָ���� �����ܳ�����Ƶ��������� ����
		///��û�������˸ñ�־��������Ч����û�������Ĳ���������Ч

		///varName:�����Ĳ���ö��
		///f0��f1: �����ñ�����ֵ��= �����ı�����ǰֵ��f0+f1
		///����ֵ���ж��ٸ������ܵ�Ӱ��
		virtual int SetWaveformVar(WaveFormParam varName,float f0,float f1){return 0;}; //���ղ�����ֵ=f0*v+f1

		///varName:�����Ĳ���ö��
		///v: �����ñ�����ֵ��= v
		///����ֵ���ж��ٸ������ܵ�Ӱ��
		virtual int SetWaveformVar(WaveFormParam varName,float v){return 0;};//���ղ�����ֵ=v

		unsigned int m_ref;
		virtual void AddRef()
		{
			++m_ref;
		};
		virtual void DelRef()
		{
			if(m_ref > 0)
				--m_ref;
		};
		virtual unsigned int GetRef(){return m_ref;};

		//�Զ�lod1�� ���ȡ��Աȶȡ��߹ⷶΧ���߹�ǿ��
		virtual float GetBrightness_AutoLod1(){return 0;};
		virtual float GetContrast_AutoLod1(){return 0;};
		virtual float GetSpecluarRange_AutoLod1(){return 0;};
		virtual float GetSpecluarIntensity_AutoLod1(){return 0;};

		virtual void SetBrightness_AutoLod1(float v){};
		virtual void SetContrast_AutoLod1(float v){};
		virtual void SetSpecluarRange_AutoLod1(float v){};
		virtual void SetSpecluarIntensity_AutoLod1(float v){};

		///����
		virtual int Exec(int a,int b,void* c){return 0;};
	};
	class CRenderStateManager:public IRenderStateManager
	{
	public:
		virtual void SetRenderState(RENDERSTATETYPE type, unsigned int value)const {};
		virtual unsigned int GetRenderState(RENDERSTATETYPE type)const { return RS_ZENABLE;};
		virtual void SetScissorRect(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)const {};

		virtual ~CRenderStateManager(){ }
	};
	/*
	class  CMoveObject: public IMoveObject//IUserObject
	{
	public:
	//OBJECT
	virtual void					Release(){delete this;};
	virtual bool					IsValid()const{return true;};

	virtual void                    SetUserPtr(void*){};
	virtual void*                  GetUserPtr(void)const {return 0;};
	//MOVE OBJECT
	virtual MODEL_TYPE				Type(){};
	virtual unsigned int			GetID(){static int moveObjNumber=0; return moveObjNumber++;};
	virtual unsigned int			PrepareDraw(){return 0;};
	virtual unsigned int			Draw(){return 0;};//����֮��͸��OBJECT��������Լ���
	virtual void*					GetAllInside(int n){return 0;};
	//�˶�����
	virtual IAnimation*				GetAnimTrack(){return 0;};
	virtual void					SetAnimTrack(IAnimation* pAnim){};
	virtual float					GetCurAnimTime(){return 0;};
	virtual void					RunAnimation(bool bRun,float fStartTime){};
	virtual bool					IsAnimationRun(){return 0;};

	virtual void					Update(){};
	virtual void					Update(float fIntervalTime){};

	virtual void                    GetAABB(float*){};
	virtual void					SetAABB(const float* boxWorld){};
	virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;};

	virtual void                    GetLocalAABB(float*){};
	virtual const H3DMat4& GetLocationMatrix()const{static H3DMat4 localMat4; localMat4.Identity();return localMat4;};
	virtual const H3DMat4& GetOrgMatrix()const{ static H3DMat4 orgMat4; orgMat4.Identity();return orgMat4;};
	virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
	virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
	virtual void					SetPosition(const H3DVec3&){};
	virtual void		            GetPosition(H3DVec3&)const{};

	virtual void					SetZRotation(float degree){};
	virtual float					GetZRotation()const {return 0;};
	virtual void					SetScale(const H3DVec3& scale){};
	virtual void	                GetScale(H3DVec3&)const{};
	virtual void					SetDirection(const H3DVec3& dir){};
	virtual void                    GetDirection(H3DVec3&)const{};

	virtual void					SetColor(const float* col, bool bUseColor){};
	virtual const float*			GetColor()const{static float ca[4]={0};return ca;};

	//��ι�ϵ���ƣ�������IMoveObject
	virtual unsigned int			GetNumChild()const{return 0;};
	virtual IMoveObject*			GetChild(unsigned int n){return 0;};
	virtual void					AddChild(IMoveObject* pMoveObj){};
	virtual IMoveObject*			GetParent(){return 0;};
	virtual void					SetParent(IMoveObject* pMoveObj){};

	virtual void					EnableSort(){};
	virtual void					DisableSort(){};
	virtual bool					IsSortEnabled(){return 0;};
	virtual void					SetLOD(size_t nLOD){};
	virtual size_t					GetLOD()const{return 0;};
	virtual size_t					GetLODNum(){return 0;};
	virtual float					GetLodDist(size_t nLod)const{return 0;};

	virtual void					EnableChildrenUpdate(bool bEnable){};

	virtual IShader*				GetShader(){return 0;};
	virtual void					SetShader(IShader* pIShader){};
	virtual void					SetEffectShader(IShader* pShader){};
	///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
	virtual int						ReloadAll()	{return 1;};

	//������
	//0����ͬ��ReloadAll
	//1��Reload Geometry
	//2��Reload ���ʣ�������ͼ��
	//3��Reload ��ͼ

	//����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ

	virtual int						ReloadFromDisk(int reloadContent){return 0;};
	///��ñ�����ʹ�õ��Դ�
	virtual unsigned int GetVideoMemUse(){return 0;};

	///�����������
	virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

	///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
	virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){};

	//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
	//����ֵΪ1��ʾ�ཻ
	virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;};

	//������ɫϵ��
	//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
	//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
	virtual int SetMaterialColorFactor(float* f,int arraysize){return 1;};

	/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
	/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
	///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
	/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
	virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;};

	//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
	virtual unsigned int			GetFlags(){return 0;};
	virtual void					SetFlags(unsigned int nFlags){};

	///���ò���lod,nLod{}��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
	virtual void SetMaterialLod(int nLod){};
	virtual int GetMaterialLod(){return 0;};
	};
	*/	


	class SIRender:public IRender
	{
		tWindowCreateInfoImp m_createInfo;
	public:
		virtual ~SIRender(){};

		HWND m_hwnd;
		HPEN hpenBlue;

		void SwapBuffer(bool) {}
		const PrintScreenInfo * GetPrintScreenInfo(void) { return 0; }
		void ReleasePrintScreenInfo(const H3DI::PrintScreenInfo *) {}

		virtual const char*				GetCoreVersionInfo(){return 0;};
		virtual void*					GetWindowHandle(){return 0;};
		virtual void					SetWorkingDirectory(const char* sDir){};

		///����������
		virtual int						CreateRenderWindow(const tWindowCreateInfoImp& info)
		{
			m_createInfo = info;
			m_hwnd = info.hWnd;  
			hpenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
			return 0;

		}
		///�������Ӵ���
		virtual int						CreateAddionalRenderWindow(HWND hwnd,int w,int h){return 0;};

		///���õ�ǰ�����Ⱦ���ڵ�ID������ֵС��0��ʾʧ�ܡ��ú����������Ƕര����Ⱦ��ʱ��ָ���������Ļ��� �ǻ��Ƶ��ĸ�������
		virtual int						SetCurrentRenderWindow(int windowID){return 0;};

		virtual void					GetCurrentWindowSetting(tWindowCreateInfoImp& info){info = m_createInfo;};

		virtual bool					InitRenderWindow(const tWindowCreateInfoImp& info)
		{
			m_hwnd = info.hWnd;  
			hpenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
			return 0;
		};

		virtual void					SetWindowResource(void* pWindowHandle,void* pDisplayContextHandle,void* pOther){};

		virtual void					ChangeDisplaySetting(const tWindowCreateInfoImp& info){};
		virtual bool					GetCurrentWindowMode(){return 0;};

		///�������ţ����һ�������Ǵ��ڵ�ID,Ĭ�ϵ�-1��ʾ��������
		virtual bool					ResizeWindowForEditor(int w,int h,int windowID=-1){return 0;};
		virtual bool					ResizeWidnowEx(int w,int h,bool windowed,bool topmost,bool popup,bool maxbox,bool pad_win_frame_size=true,int windowID=-1){return 0;};

		///sssa2000 08.04.03  !ǿ���豸lost������vista��
		virtual bool					ResetDevice(){return 0;};	
		///Set Gamma
		virtual unsigned int			SetGamma(float fGamma){return 0;};
		virtual float					GetGamma()const{return 0;};

		virtual void					ResetContext(){};

		//Render framework
		virtual void					FrameBegin(){};
		virtual void					FrameEnd(){};
		virtual void					SwapBuffer()
		{
			//HDC hdc=GetDC(m_hwnd); 
			//MoveToEx(hdc,0,0,NULL);
			//LineTo(hdc,25,25);
			//MoveToEx(hdc,25,25,NULL);//ԭ������λ��
			//LineTo(hdc,100,300);//point�ǵ�ǰ����λ��
			//ReleaseDC(m_hwnd,hdc); 
		};
		virtual unsigned int			GetClearFlags(){return 0;};
		virtual void					SetClearFlags(unsigned int nFlags){};
		virtual void					ClearScreen(){};
		virtual void					SetClearColor(const float *col){};
		/// ���ó�����������ɫ
		virtual void					SetAmbientColor(const float *col){};
		/// ���ý�ɫ��������ɫ			
		virtual void					SetActorAmbientColor(const float *col){};
		virtual void					SetWireMode(bool bWire){};
		virtual void					UpdateCamera(){};
		//!����������
		virtual bool				    CreatePhyWorld(int& outId){return 0;};
		//!����OBB
		virtual bool					CreateObbCollisionData(unsigned int name,IMoveObject* pModel,int phyId){return 0;};
		//!����triangleMesh,�����Ƕ�ȡ�������ײ�������
		virtual bool					CreateTriangleMesh(unsigned int name,const H3DVec3* points,const unsigned short* indexs,const H3DVec3* normals,int vertexNums,int triNums,int phyId){return 0;};
		//!����ASE��ʽ��triangleMesh���ڴ���������.��Ҫע����Ǹýӿ���Ҫ����2��,1�δ�����С,1����д����.��������ڴ�ָ����0ʱ��д��С
		virtual bool					LoadTriangleMesh(const char* aseName,int& vertexNums,int& triNums,H3DVec3* points,unsigned short* indexs,H3DVec3* normals){return 0;};
		//!�ͷ���ײ����
		virtual void					ReleaseCollisionData(unsigned int name,int phyId){};
		//!�ͷ���������
		virtual void					ReleaseCollisionScene(int phyId) { };
		//!������ײ����
		virtual void					SetCollisionData(const tCollisionObbInfo& obbInfo,int phyId){};
		//!������������
		virtual void					UpdatePhx(int phyId,unsigned int interval){};//��������������ⲿ����
		//!�Ͻӿ�
		virtual void					UpdatePhx(unsigned int interval=0){};//��������������ⲿ����
		//!�����������������,������Сʱ��Ƭ,ģ��ʱ��(Ϊ��ֵ��ʾȡʵ�ʵ���Ⱦʱ��,Ϊ��ֵ��ʾ�㶨ģ��ʱ��Ƭ,����ֵ
		virtual void					SetPhxPara(float fixedTime,float simTime,float gravity){};
		//!��ȡ��ײ���,����һ������ID,��������ID,���غ��䷢����ײ������ID,����,��ײ��,��ײ��,��ײ���ߵ���Ϣ
		virtual bool					GetCollisionResult(int phyId,unsigned int& collName0,int& collType0,unsigned int& collName1,int& collType1,tCollisionResultInfo& collInfo){return 0;};

		//!��ĳָ����������߲�ѯ.phxWorldId:��������id,objId:����ѯ������ID.����1��ʾ��ѯ�ɹ�,0��ʾ��ѯʧ��
		//Ŀǰֻ֧�ֺ��������͵���ײ�����ѯ
		virtual int						RayTestSingObject(float x,float y,H3DVec3& collRes,unsigned int phxWorldId,unsigned int objId) {return 0;};

		//by sssa2000 08.04.08
		virtual void SetEnableAllPhy(bool b){};
		virtual bool GetEnableAllPhy(){return 0;};
		//by lzp,�ͷŵ���������
		virtual void ReleasePhxScene(){};
		//��ȡ��Ƭ����,����һ���㹻����ڴ�,����ʵ�����Ĵ�С,idBufferSize��ʾ����Ԫ�ظ���
		virtual bool GetPhxWorldCopyInfo(int phyId,unsigned int* pIdBuffer,unsigned int idBufferSize,unsigned int& idRealSize,char* pBuffer,unsigned int bufferSize,unsigned int& realSize)const {return 0;};
		//������Ƭ����,��������Buffer,buffer��С.idBufferSize��ʾ����Ԫ�ظ���
		virtual void SetPhxWorldCopyInfo(int phyId,const H3DI::IMoveObject*const * pIdBuffer,unsigned int idBufferSize,unsigned int idRealSize,const char* pBuffer,unsigned int bufferSize,unsigned int realSize) {};

		// camera functions
		virtual void					SetViewport(const unsigned short*vp){};
		virtual void					GetViewPort(unsigned short*vp){};

		virtual void					SetPerspective(const H3DMat4& mat) { };
		virtual void					GetPerspective(H3DMat4& mat) { };
		virtual void					SetViewMatrix(const H3DMat4& mat) { };
		virtual void					GetViewMatrix(H3DMat4& mat) { };
		virtual void					SetEyePosition(const H3DVec3& pos) { };
		virtual void					GetEyePosition(H3DVec3& pos) { };
		virtual void					LookAt(const H3DVec3& eyepos, const H3DVec3& at, const H3DVec3& up) { };

		virtual void					RotateDirection(float yaw_offset,float pitch_offset,float roll_offset){};
		virtual void					SetCameraRotation(float yaw,float pitch,float roll){};
		virtual void					GetRotateDir(float& yaw,float& pitch,float& roll){};

		virtual void					GetEyeDirection(H3DVec3& vec) { };
		virtual void					SetFrustum(float fov,float ratio,float fNear,float fFar,ECameraType camType = ECT_Render){};
		virtual void					GetFrustum(float& fov,float& ratio,float& fNear,float& fFar,ECameraType camType = ECT_Render){};
		//-------------------------------------------------------
		//add by sssa2000 08.11.27
		//����2��������Ϊ�˸���������ӵģ����д����ָ������Ƿ���ÿռ�ģ��Ҳ�С��8������,�������ָ����ֵ���Ƶ�������
		//�ɹ���������ǰ4��Ϊ near,��4��Ϊfar ƽ��
		virtual void					GetCullFrustumCornerPoint(H3DVec3* p){};
		virtual void					GetRenderFrustumCornerPoit(H3DVec3* p){};
		//---------------------------------------------------------
		virtual void					PushCamera(){};
		virtual void					PopCamera(){};
		virtual void					SetReflectCamera(const float* vec4Plane){};//��һ��ƽ���뵱ǰCAMERA��Ϊ��׼���÷���CAMERA
		virtual void					CloseReflectCamera(){};
		virtual void					SetRefractCamera(const float* vec4Plane,IMoveObject* pObj=0){};
		virtual void					CloseRefractCamera(){};
		virtual void					SetFarDist(float fFar){};
		virtual int						IsAABBInFrustum(const float* boxWorld){return 0;};


		virtual IModel*					CreateModel(const char* sFileName,int nIOFlags=0)
		{
			std::string str = sFileName;
			std::transform( str.begin(), str.end(), str.begin(), ::tolower);

			if(strstr(str.c_str(), ".chr"))
				return dynamic_cast<CSkeletonModel*>(EnginePool::m_skeleton_pool->createObj(sFileName));
			else
				return dynamic_cast<CModel*>(EnginePool::m_model_pool->createObj(sFileName));
		};
		virtual IModel*					CreateModel(sCreateOp& op,const char* sFileName,int nIOFlags=0)
		{
			std::string str = sFileName;
			std::transform( str.begin(), str.end(), str.begin(), ::tolower);

			if(strstr(str.c_str(), ".chr"))
				return dynamic_cast<CSkeletonModel*>(EnginePool::m_skeleton_pool->createObj(sFileName));
			else
				return dynamic_cast<CModel*>(EnginePool::m_model_pool->createObj(sFileName));
		};

		//*ppModel will be fill after asyn loading finished, but before that , *ppModel ={} !
		virtual  unsigned int			CreateModelAsyn(const char* sFileName,H3DI::IModel** ppModel){return 0;};

		virtual IModel*					CreateModel(const char* sFileName, const H3DMat4& matLocate, IScene* pScene, int nLayer) {return 0;};
		virtual IModel*					CreateActor(const char* sActorName, bool bMale){return 0;};
		virtual IModel*					CreateActor(sCreateOp& op,const char* sActorName, bool bMale)
		{
			return dynamic_cast<CActor*>(EnginePool::m_actor_pool->createObj("actor", sActorName));
		};
		virtual unsigned int			CreateActorAsyn(const char* sActorName, H3DI::IModel** ppModel, bool male = true){return 0;};
		virtual IAvatarSkeletonModel*	CreateAvatarSkeletonModel(const char* sActorName, bool bMale){return 0;};
		virtual IAvatarSkeletonModel*	CreateAvatarSkeletonModel(sCreateOp& op,const char* sActorName, bool bMale)
		{
			return dynamic_cast<CAvatarSkeletonModel*>(EnginePool::m_avaskeleton_pool->createObj("ava_ske_model", sActorName));
		};
		virtual unsigned int			CreateAvatarSkeletonModelAsyn(const char* sActorName, H3DI::IAvatarSkeletonModel** ppModel, bool male){return 0;};

		virtual IModel*					CreateEquipment(const char* sEquipmentName){return 0;};
		virtual IReflectModel*			CreateReflectModel(const char* sFileName){return 0;};
		virtual void					CleanAllModels(){};
		virtual unsigned int			RenderModel(IModel* pModel){return 0;};

		virtual void					Flush(){};

		//draw index and vertex buffer
		virtual IVB*					GetDynamicVB(){
			static CVB vb;
			return &vb;
		};

		virtual void					DrawIndex(PRIMITIVE_TYPE nTriangleType,unsigned int nElemNum,unsigned int nVertNum,unsigned int nNormalOffset,unsigned int nTexcoordOffset,unsigned int nColorOffset,unsigned int nTexcoordOffset2){};


		virtual void					DrawTriangle(float* v){};
		virtual void					DrawQuad(float*v ){};
		virtual void					DrawBBox(const float* box,bool bIsWire,const float* col4,bool bIsBlend){};


		//shader functions

		virtual bool					OpenShaderLib(const char* sFileName,int nIOFlags=0){return 0;};
		virtual unsigned int 			OpenShaderLibAsyn(const char* sFileName,int scale, int compress){return 0;};
		virtual void					CleanShaderLib(){};
		virtual void					ReloadAllShader(){};
		virtual unsigned int			GetCurShaderLibNum(){return 0;};
		virtual const char*				GetCurShaderLibShaderName(unsigned int n) {return 0;};
		virtual IShader*				GetShader(const char* sName){return 0;};
		virtual IShader*				GetShaderThreadSafe(const char* sName){return 0;};
		virtual void					SetShader(IShader* pShader){};
		virtual unsigned int			GetNumShaders(){return 0;};
		virtual const char*				GetShaderName(unsigned int n){return 0;};
		virtual IShader*				GetShader(unsigned int n){return 0;};
		virtual bool					UpdateTexture(IShader* pShader,unsigned int nPass,unsigned int nStage,unsigned char* pTexBuffer,
			unsigned int x,unsigned int y,unsigned int z,unsigned int w,unsigned int h,unsigned int l){return 0;};
		virtual void					CopyTexture(IShader* pShader,unsigned int nPass,unsigned int nStage,
			unsigned int x,unsigned int y,unsigned int z,unsigned int w,unsigned int h,unsigned int l){};

		//���ⲿ�ļ�����һ������
		virtual ITerrain*				CreateTerrain(bool EditMode = false){return 0;};	

		virtual void					SetTerrainHorizonCulling(int nHorizonDist,int nTestDist){};
		virtual void					GetTerrainHorizonCulling(int& nHorizonDist,int& nTestDist){};
		virtual int						IsTerrainHorizonCulling(){return 0;};
		virtual int						HorizonTestBBox(const float* box){return 0;};
		virtual void					RenderHorizon(){};

		virtual ISpecialEffectManager* CreateEffectManager() {return &m_specialEffectMng;};

		//////////////////////////////////////////////////////////////////////////
		//INDOOR
		//
		//
		virtual IInDoor*				CreateInDoor(const char* sFileName){return 0;};

		//////////////////////////////////////////////////////////////////////////
		//
		//light

		virtual ILight*					CreateLight(LIGHT_TYPE type){return 0;};
		//���еĲ������ø����ε�ȫ�ֹ���
		virtual void					SetGlobalLight(ILight* pLight){};
		//�õ�Ӱ����ε�ȫ�ֹ�ӿ�
		virtual ILight*					GetGlobalLight(){return 0;};


		virtual IBillboard*				CreateBillboard(BILLBOARD_TYPE type,BILLBOARD_PROPERTY property){return 0;};



		//////////////////////////////////////////////////////////////////////////
		//Pipe Functions
		virtual void					PushModel(IModel* pModel){};
		//��������CBillboard,CUserObject
		virtual void					PushMoveObject(IMoveObject* pMoveObj){};
		virtual void					PushNewPipe(PIPE_TYPE type,unsigned int flags,ILight* pLight, PipeSettingFunc pStartFunc=0,PipeSettingFunc pEndFunc=0){};
		virtual void					ProcessCurrentPipe(){};//�����������������Ȳ���,���йܵ�����Ҫ�˲���
		virtual unsigned int			RenderPushedPipes(){return 0;};//�����Ѿ�ѹ�������PIPE 
		virtual unsigned int			RenderTransparentPipe(){return 0;};
		virtual void					CleanPushedObjsAndSubModels(){};
		virtual void					SetTransparentState(){};
		virtual void					ClearTransparentState(){};


		virtual void					GlareProcess(bool bUse,float explosure){};


		//pre-lighting interface by Garuda 20100308
		virtual void					PushObject(IMoveObject* pMoveObj) { };
		virtual void					Render() { };
		virtual void					EnableShadow(bool bEnable) { };
		virtual IPrePassLight*			CreatePrePassLight(LIGHT_TYPE type)
		{ 
			return dynamic_cast<IPrePassLight*>(EnginePool::m_light_pool->createObj("light"));
		};

		//////////////////////////////////////////////////////////////////////////
		//Shadow
		//virtual void					AddShadowTechnique(H3DI::SHADOWMAP_TECHNIQUE t){};
		//virtual void					EndShadowTechnique(H3DI::SHADOWMAP_TECHNIQUE t){};	

		virtual void					PushShadowmapModel(IModel* pIModel){};
		virtual void					PushFakeShadowModel(IModel* pIModel){};
		virtual void					PushFakeShadowModel(IMoveObject* pIObj){};
		virtual void					RenderShadowmap(){};
		virtual void					RenderFakeShadow(){};
		virtual void					RenderTerrainShadowmap(){};
		virtual void					RenderTerrainFakeShadow(){};
		virtual void					SetShadowColor(const float* vec4Col){};
		virtual void					SetShadowmapLightDir(const float* vec3Dir){};
		virtual void					SetShadowmapRadius2(float fRadius){};
		virtual void					PushShadowmapReceiver(IModel* pIModel){};
		virtual void					PushFakeShadowReceiver(IModel* pIModel){};
		virtual void					SetShadowTerrainReceiver(ITerrain* pTerrain){};


		//////////////////////////////////////////////////////////////////////////
		//PBuffer
		virtual bool					Active2ndRenderTarget(){return 0;};
		virtual void					Disable2ndRenderTarget(){};
		virtual bool					Active3rdRenderTarget(){return 0;};
		virtual void					Disable3rdRenderTarget(){};
		virtual bool					Active4thRenderTarget(){return 0;};//for shadowmap
		virtual void					Disable4thdRenderTarget(){};
		virtual bool					ActiveRenderTarget(int n){return 0;};
		virtual void					DisableRenderTarget(int n){};
		virtual unsigned int			GetRenderTargetID(int n){return 0;};
		virtual unsigned int			RenderScreenToBuffer(int n){return 0;};

		//////////////////////////////////////////////////////////////////////////
		//Fullscreen Glow
		virtual void					RenderFullScreenGlow(float fBright){};
		virtual void					RenderFullScreenEffect(int n,const float* vec4Col){};

		//============================ FOG =========================

		virtual void					EnableGlobalFog(){};
		virtual void					DisableGlobalFog(){};
		virtual void					SetGlobalFogParam(FOG_TYPE type,const float* vec4Color,float fNear,float fFar,float fDensity){};


		/////////////////////////////// User Object ///////////////////////////////////////////

		virtual void					CreateHardcoreMoveObjectPtr(IMoveObject* pIObj){};
		virtual void					ReleaseHardcoreMoveObject(IMoveObject* pIObj){};

		//////////////////////////////////////// Particle VB //////////////////////////////////
		virtual unsigned int			GetMaxParticleNum(){return 0;};
		virtual char*					LockParticleVB(){return 0;};
		virtual void					UnLockParticleVB(int nParticleNum){};

		virtual void					DrawParticle(int nParticleNum, IShader* pIShader, IParticleVB* pVB, unsigned int offSet){};


		///////////////////////////////////OCCLUSIdON QUERY///////////////////////////////////////
		virtual int						OcclusionQuery(const float* boxWorld){return 0;};
		virtual int						OcclusionQuery(IModel* pModel){return 0;};

		/*
		ĳЩ�Կ�ֻ֧��16bit����������, Ϊ�˼��ٴ����ϵ��鷳, �ͻ��˵�����bufferͳһʹ��unsigned short��ʽ
		*/
		virtual void					DrawIndex(PRIMITIVE_TYPE nTriangleType,unsigned int nElemNum,unsigned int nVertNum,unsigned int nNormalOffset,
			unsigned int nTexcoordOffset,unsigned int nColorOffset,unsigned int nTexcoordOffset2,void* pVBuffer,unsigned short* pEBuffer){};


		//////////////////////////////////////////////////////////////////////////
		//THREAD
		//
		//
		//!type 0: IO Thread 
		//!type 1: assitant thread1
		//!type 2: assitant thread2
		virtual int 					StopEngineThread(int type){return 0;};
		virtual int						StartEngineThread(int type){return 0;};
		//����0 ��������������ERR CODE
		virtual int						IsThreadTaskOK(unsigned int task_id){return 0;};
		virtual int						GetThreadTask(int thread_id=0){return 0;};
		//����0 ��û�з������ã�1 �ɹ��������������ɾ���� 2 �����Ѿ���� 3�� �������Ѿ���ɣ�����ʧ����
		virtual int						CancleTask(unsigned int task_id,int threadid=0){return 0;};
		virtual void					SetAsynMode(int n){};
		virtual int						GetAsynMode(){return 0;};
		virtual void					LockRenderer(){};
		virtual void					UnlockRenderer(){};
		virtual bool					IsRendererLocked(){return 0;};
		virtual void					MakeCurrent(){};
		virtual void					CleanFIFO(){};

		//==============================================

		//!�õ���ǰ�ؿ�,����
		virtual ILevel* GetLevel(void)const {return 0; };
		//!�������
		virtual ICamera* CreateCamera(void) 
		{
			return dynamic_cast<CCamera*>(EnginePool::m_camera_pool->createObj("camera")); 
		};
		//!������Ⱦ���
		virtual void SetRenderCamera(ICamera* pCam) { };
		//!�õ���Ⱦ���
		virtual H3DI::ICamera* GetRenderCamera(void)const {static CCamera renderCamera; return &renderCamera; };
		//!����culling���
		virtual void SetCullingCamera(ICamera* pCam) { };
		//!�õ�culling���
		virtual H3DI::ICamera* GetCullingCamera(void)const {static CCamera cullingCamera; return &cullingCamera; };
		//!��AABB
		virtual void DrawAABB(const float* aabb, const float* color, float lineWidth)const { };
		//!���߶�
		virtual void DrawSegment(const H3DVec3& begin, const H3DVec3& end, const float* color, float lineWidth)const { };
		//!����
		virtual void SetRenderMode(ERenderMode) { };

		//!��������VB
		virtual IParticleVB* CreateParticleVB(void) {return 0; };
		//!pushһ�����ӽ�����Ⱦ
		virtual void PushParticle(H3DI::IMoveObject* pObj) { };

		//!��һ��������
		virtual bool                    OpenActionLib(const char* fileName)const {return 0;};
		//!��ȡһ�������ļ�
		virtual bool                    LoadAction(const char* actName, ISkeletonModel* p)const {return 0;};
		virtual unsigned int			LoadActionAsyn(const char* actName, H3DI::ISkeletonModel* p){return 0;};
		//!ȫ��blur
		virtual void                    FullScreenBlur(unsigned int filterWidth, unsigned int filterPass, const unsigned int* viewPort = 0) { };
		//!��ʼ��Ⱦ����
		virtual void                    MotionBlurBegin(const float* pColor) { };
		//!������Ⱦ����
		virtual void                    MotionBlurEnd(void) { };
		//!˥������
		virtual void                    DarkenMotionBlurMap(float rate) { };
		//!��ȡ�ؿ��ļ�
		virtual ILevel*                 LoadLevel(const char* fileName) {return 0; };
		virtual unsigned int			LoadLevelAsyn(const char* fileName,ILevel** ppLevel){return 0;};
		//!pushһ���ؿ�������Ⱦ
		virtual void                    PushLevel(ILevel* p) { };
		//!��Ⱦ�ؿ�����Ӱ
		virtual void                    RenderLevelShadowMap(ILevel*) { };
		//!ִ������,����չ�ӿ�
		virtual void					Exec(unsigned int code, const char* argument, unsigned int argumentLength, char* buffer, unsigned int bufferLength) { };
		//!��ѯ�ӿ�
		virtual void*					QueryInfo(int QueryCode,void* param1,void* param2,void* param3,void* param4,void* param5){return 0;};
		//!�����Ƿ�ʹ�ö�����,IRender��ʼ����ʱ�����һ�μ���,Ĭ��Ϊ��ʹ��
		virtual void					SetUseActionLib(bool val) { };
		//!�Ƿ�ʹ�ö�����
		virtual bool					UseActionLib(void)const {return 0;};
		//!�����Ƿ�ϲ�Actor��Bodypart��ͼ,IRender��ʼ����ʱ�����һ�μ���,Ĭ��Ϊ��ʹ��
		virtual void                    SetMergBodyPart(bool val) { };
		//!�Ƿ�ϲ�bodypart
		virtual bool                    MergBodyPart(void)const {return 0;};
		//���motionblur H3DMap������,һ��������ı�λ�õ�ʱ�����
		virtual void                    ClearMotionBlur(void) { };
		//!����SkeletonModel��ʵ���Ƿ�ʹ�ö�춯��,Ĭ��Ϊ��ʹ��
		virtual void                    SetUseActionChannel(bool val) { };
		//!SkeletonModel��ʵ���Ƿ�ʹ�ö�춯��
		virtual bool                    UseActionChannel(void)const {return 0;};
		//!�ڶ�����ģʽ��,��ȡһ�鶯��
		virtual void                    PreLoadActions(const char** actNames, unsigned int actCount, ISkeletonModel* pSKL) { };
		//!�ڶ�����ģʽ��,���ǰ������������ж��������bOnlyClearUnUsedΪtrue��ֻ�����δʹ�õĶ�����
		virtual void                     ClearAllActions(bool bOnlyClearUnUsed = true) { };
		//!���¶�ȡ�������е�һ������(��ԭ�����ļ���)
		virtual bool                     ReloadAction(const char* actName, ISkeletonModel* pSKL) {return 0;};

		//!�����Ƿ�������ģ��
		virtual void                     SetUseCloth(bool val) { };
		//!�Ƿ�������ģ��
		virtual bool                     UseCloth(void)const {return 0;};
		//!�����Ƿ�����ʹ���������б�
		virtual void                     SetUseTriList(bool val) { };
		//!�Ƿ�����ʹ���������б�
		virtual bool                     GetUseTriList(void)const {return 0;};


		//!������ͼ���Ų���
		virtual void                     SetImageScaleFactor(unsigned int) { };
		//!�õ���ͼ���Ų���
		virtual unsigned int          GetImageScaleFactor(void)const {return 0; };
		//!������õ���ͼ,shader����Դ
		virtual unsigned int          ClearMaterialLib(void) 
		{
			EnginePool::clearMaterailLib();
			EnginePool::dump();
			return 0; 
		};
		//!�õ�״̬������
		virtual const IEngineMonitor*    GetEngineMonitor(void)const { return 0;}; 
		//!�����Ƿ����ȶ�ȡ������
		virtual void					  SetFavorBinary(bool val) { };
		//!�Ƿ����ȶ�ȡ������
		virtual bool                      FavorBinary(void)const {return 0;};
		//!�õ�����ʱģʽ
		virtual ERuntimeMode       GetRuntimeMode(void)const { return RUNTIME_DX9; };
		//!push�������
		virtual void					  PushViewMatrix(void)const { };
		//!pop�������
		virtual void                      PopViewMatrix(void)const { };
		//!pushͶӰ����
		virtual void                      PushProjectionMatrix(void)const { };
		//!popͶӰ����
		virtual void                      PopProjectionMatrix(void)const { };
		//!�õ���Ⱦ״̬������
		virtual const IRenderStateManager* GetRenderStateManager(void)const {
			static CRenderStateManager renderStatusMng;
			return &renderStatusMng;};
			//!����һ����ͼ
			/**
			client������texture����RT��,Ҫִ��addref����,�����������ڴ��ʱ����ܻ�������ͼɾ��
			����ǰ,Ҫ��ִ��delref,��ִ��release
			���µ�һ�������Ǵ���һ��ָ����С��texture���ڶ�����ͨ�������ļ�����һ��texture,�ڶ���������ʾmipmap�ĵȼ� 0��ʾ�Զ�����
			*/
			virtual ITexture*               CreateTexture(unsigned int& width, unsigned int& height, ETextureFormat fmt, bool mipmap) 
			{
				CTexture* ct = dynamic_cast<CTexture*>(EnginePool::m_texture_pool->createObj("texture_name"));
				ct->setWH(width, height);
				return ct; 
			};
			virtual ITexture*               CreateTextureFromFile(const char* filename, unsigned int numMips=0) 
			{
				CTexture* ct = dynamic_cast<CTexture*>(EnginePool::m_texture_pool->createObj(filename));
				ct->setWH(32, 32);
				return ct; 
			};
			//!����һ��RT
			virtual IRenderTarget*      CreateRenderTarget(unsigned int& width, unsigned int& height, ETextureFormat fmt, ERenderTargetType type) 
			{
				CRenderTarget* rt = dynamic_cast<CRenderTarget*>(EnginePool::m_rendertarget_pool->createObj("render_target"));
				alwaysAssert(rt);
				rt->setWH(width, height);
				return rt; 
			};
			//!�����Ƿ���ͼѹ����DXT��ʽ
			virtual void                      SetCompressTexture(bool res)const { };
			//!�Ƿ���ͼѹ����DXT��ʽ
			virtual bool                      CompressTexture(void)const {return 0;};
			//!�õ�ƽ��ͶӰ����
			virtual void                       GetOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far, H3DMat4& out)const { };
			//!�޸ĵ�ǰview����
			virtual void                       TranslateViewMatrix(float x, float y, float z)const { };
			//!��ת��ǰview����
			virtual void                       RotateViewMatrix(float degree, float x, float y, float z)const { };
			//!�õ���Ļ����ϵ�µ�����
			virtual void                       GetScreenPos(const H3DVec3& objPos, const H3DMat4& worldViewMat, const H3DMat4& projMat, 
				unsigned int x, unsigned int y, unsigned int width, unsigned height, 
				float minZ, float maxZ, H3DVec3& screenPos)const { };
			//!������ͼ����
			virtual void                        SetTextureParams(bool scale, bool compress, bool mipmap)const { };
			//!�õ���ͼ����
			virtual void                        GetTextureParams(bool& scale, bool& compress, bool& mipmap)const { };
			//!����ȫ��������ģʽ
			virtual void						ToWindowedMode(){};
			virtual void						ToFullScreenMode(){};
			//!����ȫ�������ģʽ
			virtual void                         SetAAMode(EAAMode aa) { };
			//!�õ�ȫ�������ģʽ
			virtual EAAMode              GetAAMode(void)const {return AA_Disable; };
			//!�õ�֧�ֵĿ��������
			virtual unsigned int           GetSupportedAAMode(void)const {return 0; };
			//!���ø������Թ���ģʽ
			virtual void                          SetAFMode(EAFMode) { };
			//!�õ��������Թ���ģʽ
			virtual EAFMode               GetAFMode(void)const {return AF_Disable; };
			//!�õ�֧�ֵĸ������Թ���ģʽ
			virtual unsigned int           GetSupportedAFMode(void)const { return AF_Disable;};
			//!�����Ƿ�����ֱͬ��
			virtual void                          SetVSync(bool val) { };
			//!�Ƿ�����ֱͬ��
			virtual bool                          GetVSync(void)const {return 0;};
			//!������Ӱģʽ
			virtual void                          SetShadowQuality(EShadowQuality q) { };
			//!�õ���Ӱģʽ
			virtual EShadowQuality    GetShadowQuality(void)const { return Shadow_Off;};
			//!�����Ƿ�������
			virtual void                          EnableCloth(bool val) { };
			//!�Ƿ�������
			virtual bool                          ClothEnabled(void)const { return 0;};
			//!������ʾ����
			virtual void                         ApplyVideoSettings(void) { };
			virtual void						 ApplyVideoSettings2(void) { };
			//!�õ���ǰ�ķֱ���
			virtual void						 GetCurrentReslution(int& w,int& h){};
			//!ж������bp3��ͼ
			virtual void                         UnloadBodyPartImages(void) { };
			virtual unsigned int                 UnloadBodyPartImagesAsyn(void) {return 0; };
			//!�õ�profile����
			virtual void                         GetPerfData(tPerfData& data)const { };
			//!��������
			virtual void                         SetGravity(const H3DVec3& g) { };
			//!�õ�����
			virtual const H3DVec3&      GetGravity(void)const {  static H3DVec3 v3;v3.Set(0,0,0); return v3;};
			//!ж�ص�һһ��bp3��ͼ
			virtual void                         UnloadSingleBodyPartImages(const char* bpName,int ActorStamp) { };

			//!�����¼�������
			virtual void                          SetEventHandler(IEngineEventHandler* pHandler) { };
			//!�õ��¼�������
			virtual IEngineEventHandler* GetEventHandler(void)const { return 0; };

			//!ץ��
			virtual bool SaveScreenToFile(const char* ){return 0;};
			//����IImage
			virtual IImage* GetImageInterface(void){return 0;};

			virtual IPerfMonitor* GetPerfMonitor(){return 0;};

			//by sssa2000 090330
			//Ϊ�༭����ӵ��½ӿ�
			virtual void beginWireFrameDraw(){};
			virtual void endWireFrameDraw(){};
			virtual void drawPhy(){};
			//add by liren
			//�����η������
			virtual void PushTerrain(H3DI::ITerrain *pTerrain) { };
			//!add by liren

			//add by liren
			//�ڵ����ϴ���һ����Ƭ
			virtual	IModel*			GenerateSmallPatch(float length,float width,ITerrain *pTerrain) {return 0; };
			//!add by liren
			//Ӳ��ѡ��
			//����Ӳ��ѡ�񣬴���һ����Ļ���Σ���ѡ��ʽ,
			virtual	bool			BeginHardwareSelect(const int *pRect,bool bSelectFirst, int WindowID) {return 0; };
			//����һ���������Ե�ID
			virtual void			PushHardwareSelectObject(IMoveObject *pobject,unsigned int nId) { };
			//���������ѡ��
			virtual	void			ProcessMoveObjectSelect() { };
			//!ѹ��һ��Id���������Զ��嶥�㣬�ߺ�VB��
			virtual void			PushSelectID(int ID) { };
			//!����
			virtual int				GetSelectID(void)const {return 0; };
			//����ѡ�񣬷��ؽ��
			virtual	int				EndHardwareSelect(unsigned int *pRetIds,int num) { return 0;};
			//!���þ�ͷ���β���
			virtual void			SetLensParameters(const tLensEnv& lenEnv,const tLensFlareParameters* lensPara,unsigned int nums) { };
			//!����̫���߶�,Ĭ��֧��һ��̫��
			virtual void			SetSunHeight(float height){};
			//!����̫��λ��,�༭����
			virtual void			SetSunPos(const H3DVec3& pos){};
			//!��ȡ̫��λ��,�༭��debug��
			virtual void			GetSunPos(H3DVec3& pos){};

			//��(����Ⱦϵͳ)
			virtual H3DI::IFog*					CreateFog() {return 0; };
			//����ȫ����Ŀǰȫ������Ҫ������
			virtual void SetGlobalFog(float r,float g,float b,float density,float start,float fogmin){};
			//����������ͼ
			virtual int ReloadTexture(const char* filename){return 0;};

			//������ؽӿ�
			//���ɾ������Ч���Ľӿ�
			virtual bool					AddPostProcess(const char *FileName) {return 0;};

			virtual bool					DelPostProcess(const char *FileName) {return 0;};
			//����
			virtual void					EnablePostProcess(bool bEnable) { };
			//���ò���,������Ч�����ɵ��Ĳ���,������ӿ����á�
			//��������Щ������ÿ������ʲô���壬������ĵ��ṩ��
			virtual void					SetPostProcessParams(const char* name,const void *pParmeValue) { };

			//���ú���Ӱ��Ĵ��ڡ�
			virtual void					SetPostProcessWindowId(int){};
			//��ú���Ӱ��Ĵ��ڡ�
			virtual int						GetPostProcessWindowId(){return 0;};



			//�������ⳡ����ȫ�ֻ���
			virtual void SetOutDoorEnvInfo(const OutDoorEnvInfo& info){};
			virtual void GetOutDoorEnvInfo(OutDoorEnvInfo* info){};

			//�������ʱ��Ϣ
			virtual void GetRuntimeInfo(IRuntimeInfo* pRes){};

			//����ƽ����룬���Χ�������ɢԲ�뾶���ø�����������
			virtual void SetFocalParam(float fFocalplane,float fDOFRange,float fConfusion) { };
			//�õ���ƽ����룬���Χ�������ɢԲ�뾶���ø�����������
			virtual void GetFocalParam(float &fFocalplane,float &fDOFRange,float &fConfusion) { };


			//�ͻ�����PushCamera��PopCamera�����������������ֱ𽫾������ѹջ�򵯳�ջ��
			virtual void PushFocalParam() { };
			virtual void PopFocalParam() { };
			//��һ��ģ����ӵ�����ģ��ʵ����
			virtual	void AddSkeletonModelToInstancing(H3DI::ISkeletonModel *pSkeletonModel) { };
			//����ģ����ӵ�����ģ��ʵ����
			virtual void ProcessSkinedInstancing() { };
			//�õ��������ͼ
			virtual IRenderTarget* GetSceneDepthUsedInPostprocess() {return 0; };

			//�����Ƿ����ӳٹ��ջ���
			virtual void EnableLightPrePassRendering(bool b){};

			//����һ������ʵ���ļ��� �Լ�����������ֵõ�INewShader
			virtual H3DI::INewShader* GetNewShader(const char* filename,const char* matName)
			{
				std::string sn = filename;
				sn += "@";
				sn += matName;

				return dynamic_cast<H3DI::INewShader*>(EnginePool::m_shader_pool->createObj(sn.c_str()));
			};

			//�õ��������
			virtual int GetEngineLastError(){return 0;};

			//�õ�������������
			virtual const char* GetErrorDesc(int errCode){return 0;};

			//���һ������ģ�ͣ�����ģ��ÿ֡���������ã���������þ�û��ģ�ͱ�����
			virtual int AddHighLightModel(const H3DI::IModel* pModel){return 0;};

			virtual unsigned int GetRenderFrameCount() { return 0;};

			//���ôα���ɢ����ɫ
			virtual void		SetTransmissionColor(const float* col) { };

			//�������þ�������ĺ������òη�ʽ
			virtual void					SetPostProcessParams1(const char* name,const void *pParmeValue) { };

			///��������ʹ����
			virtual void					SetUser(EngineUserDefine u){};
			//��ʼDrawIndex
			virtual void					BegineDrawIndex() { };

			//����DrawIndex
			virtual void					EndDrawIndex() { };

			///��ʼ��ѯ��֡�Դ�����
			virtual void BeginCurrentFrameVideoMemQuery(){};

			///������ѯ��֡�Դ�����
			virtual unsigned int EndCurrentFrameVideoMemQuery(){return 0;};

			///�����Ƿ���2d���ơ�
			virtual void Set2DMode(bool b){};

			///push 2d����
			virtual void Push2DObj(IMoveObject* pMoveObj){};

			///Render 2d����
			virtual void Render2DObj(){};
			//����һ���������
			virtual IPostProcessEffect*		CreatePostProcessEffect(){return 0;};
			//�ͷ�һ���������
			virtual void					ReleasePostProcessEffect(IPostProcessEffect *pIEffect){};

			//�õ�Ĭ�ϵĺ������
			virtual IPostProcessEffect*		GetDefaultPostProcessEffect(){return 0;};

			//�õ���ǰ�ĺ������
			virtual IPostProcessEffect*		GetCurrentPostProcessEffect(){return 0;};

			//���õ�ǰ�ĺ������
			virtual void					SetCurrentPostProcessEffect(IPostProcessEffect *pIEffect){};
			//�õ���ǰ����
			virtual ITerrain*				GetActiveTerrain() {return 0; };
			//������Դ�󶨵�������Ƿ����ı�־
			virtual	bool					GetLightShaftEnable() {return 0; };
			//�������Դ�󶨵�������Ƿ����ı�־
			virtual	void					SetLightShaftEnable(bool bEnable) { };
			//// !ʹ���²���ϵͳDraw Index
			virtual void					DrawIndexNew(PRIMITIVE_TYPE nTriangleType,unsigned int nElemNum,unsigned int nVertNum,unsigned int nNormalOffset,unsigned int nTexcoordOffset,unsigned int nColorOffset,unsigned int nTexcoordOffset2)
			{
				if(nTriangleType == H3DI::TRIANGLE_STRIP)
				{

				}
			};
			///! ������
			virtual int						OnCommandLine(const char* commandLine) {return 0; };
			//ʹ��Ӳ��ѡ��
			virtual void					SetUseHardWareSelection(bool buse) { };
			//��ȡ�Ƿ�ʹ��Ӳ��ѡ��
			virtual bool					UseHardWareSelection() {return 0;};

			//��ȡȫ�ֵ���������
			virtual void* GetGlobalPerfData(GLOBAL_PERFDATA_TYPE t){return 0;};

			/// ���ز��ʵĶ���Ч��
			virtual void EnableMaterialAnimation(bool b) { };

			// ���õ�ǰ��Ⱦģʽ����ͨ���߿���ͨ+�߿�
			virtual void					SetFillMode(EFILLMODE mode) { };
			// �����߿����ɫ
			virtual void					SetWireframeColor(const float* color)  { };

			/// ��Ӧ�²���INewShader������/// ����DrawIndex��Ҫʹ�õ�INewShader
			virtual void					SetNewShader(H3DI::INewShader* pShader) { };

			///��ʼ���õ�ǰ����Ҫʹ�õĵƹ�
			virtual void BeginSetCurrentLight(){};

			//���õ�ǰ����Ҫʹ�õĵƹ�
			//t�������⻹�������
			//pLight����Դָ��
			//����ֵ���������-1��ʾ���������涨�����ò��ɹ������ش���0��ֵ��ʾ�ɹ�
			virtual int SetCurrentLight(H3DI::IPrePassLight* pLight){return 0;};

			///�������õ�ǰ����Ҫʹ�õĵƹ�
			virtual void EndSetCurrentLight(){};

			///��ȡ��ǰ����Ҫʹ�õĵƹ������
			virtual int GetCurrentLightNum(H3DI::LightAffectParam t){return 0;};

			//��ʼ��ViewPort�л�����
			virtual void BeginViewPort() { };
			//������ViewPort�л�����
			virtual void EndViewPort() { };

			//��ȫ�ֵĲ�����
			virtual int OpenPhxParaLib(const char** paraLibs,int bufferSize,int phxParaType) { return 0;};
			//��ȡָ�����Ƶ��������
			virtual int GetPhxParaFromLib(const char* pName,void** paraData,int bufferSize,int phxParaType) {return 0; };
			//��ָ�����Ƶ�����������뵽������
			//opType: 0,������ 1,���ǡ�����0��ʾʧ�ܡ�����1��ʾ�ɹ�.��ͬ������Ҫ��ʾ�Ƿ񸲸�
			virtual int AddPhxParaToLib(const char* pName,void** paraData,int bufferSize,int phxParaType,int opType) {return 0; };

			//��ȡ����ѡʱ�����߷���,ʹ�õ�ǰ����Ⱦ����Ĳ���
			//point��dir����x��y��z��������
			virtual int GetMouseRayDir(int mouse_x,int mouse_y, H3DVec3* pos, H3DVec3* dir){return 0;};

			virtual int GetMouseRayDirEx(const H3DVec3& eye_position, const H3DVec3& camera_direction, const H3DVec3& camera_up, float f_near,
				float f_far, unsigned int screen_width, unsigned int screen_height, float fovX, int mouse_x,int mouse_y, H3DVec3* pos, H3DVec3* dir){return 0;};

			//�����Χ���ļ�������ֵ1��ʾ�ɹ���С��0���ɹ�
			virtual int LoadCustomBoundingVolume (const char* filename){return 0;};

			//!����һ��fxinfo
			virtual IFxInfo* CreateAFxInfo(){return 0;};
			//����һ��ILevel
			virtual	ILevel* CreateLevel(const char * name) 
			{
				return dynamic_cast<ILevel*>(EnginePool::m_level_pool->createObj(name, name));
			};

			// ����ǰ֡��Ⱦ����ͼ
			// origPathName: ԭ��ͼ��·����,���ڻ�ø�ʽ����С
			virtual bool RenderToTexture(const char* origPathName, const char* pathName) {return 0;};
	protected:
		SpecialEffectManager m_specialEffectMng;
	};

}
#endif