#ifndef ENGINESHELL
#define ENGINESHELL
// C运行时头文件
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>

// STL容器
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
		///得到pass数量
		virtual unsigned int GetPassNumber(){return 0;};

		///得到某个变量的值,浮点数类型，返回值为1表示成功 0表示失败
		virtual int GetNodeVarValue_Float(const char* varName,float& res){return 0;};
		virtual int GetNodeVarValue_Int(const char* varName,int& res){return 0;};
		virtual int GetNodeVarValue_Float4(const char* varName,float& f1,float& f2,float& f3,float& f4){return 0;};
		virtual int GetNodeVarValue_Str(const char* varName,const char*& res){return 0;}; //res为外部传入的一个指针，引擎内部会修改这个指针的值
		virtual int GetNodeVarValue_Matrix44(const char* varName,float* mat){return 0;}; //mat为至少16个元素的数组,mat的空间由使用者分配

		virtual int SetNodeVarValue_Float(const char* varName,float res){return 0;};
		virtual int SetNodeVarValue_Int(const char* varName,int res){return 0;};
		virtual int SetNodeVarValue_Float4(const char* varName,float f1,float f2,float f3,float f4){return 0;};
		virtual int SetNodeVarValue_Str(const char* varName,const char* res){return 0;};
		virtual int SetNodeVarValue_Matrix44(const char* varName,float* mat){return 0;}; //mat为至少16个元素的数组,mat的空间由使用者分配

		virtual void GetMaterialInfo(MaterialInfo* mtlInfo) {};

		///设置贴图给Shader,返回1表示成功，0表示失败
		virtual int SetTexture(const char* varName,H3DI::ITexture* pTex){return 1;};

		/// 得到此材质使用的ITexture，0表示没有
		virtual H3DI::ITexture* GetTexture(const char* varName) {return 0;};

		///放弃一个变量所有的修改，恢复初始值
		virtual int ResetVar(const char* varName){return 0;};
		///得到一个shader所有的变量数量
		virtual int GetAllNodeVarNumber(){return 0;};
		///得到一个变量名字的类型字符串
		virtual const char* GetNodeVarType(const char* varName){return 0;};
		///得到材质名称
		virtual const char* GetMaterialName(){return 0;};


		///设置振荡器参数。该接口对该材质的所有的，在编辑器中指定了 “接受程序控制的振荡器”的 振荡器
		///对没有设置了该标志的振荡器无效，对没有振荡器的材质设置无效

		///varName:振荡器的参数枚举
		///f0，f1: 振荡器该变量的值将= 振荡器改变量当前值×f0+f1
		///返回值：有多少个振荡器受到影响
		virtual int SetWaveformVar(WaveFormParam varName,float f0,float f1){return 0;}; //最终参数的值=f0*v+f1

		///varName:振荡器的参数枚举
		///v: 振荡器该变量的值将= v
		///返回值：有多少个振荡器受到影响
		virtual int SetWaveformVar(WaveFormParam varName,float v){return 0;};//最终参数的值=v

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

		//自动lod1的 亮度、对比度、高光范围、高光强度
		virtual float GetBrightness_AutoLod1(){return 0;};
		virtual float GetContrast_AutoLod1(){return 0;};
		virtual float GetSpecluarRange_AutoLod1(){return 0;};
		virtual float GetSpecluarIntensity_AutoLod1(){return 0;};

		virtual void SetBrightness_AutoLod1(float v){};
		virtual void SetContrast_AutoLod1(float v){};
		virtual void SetSpecluarRange_AutoLod1(float v){};
		virtual void SetSpecluarIntensity_AutoLod1(float v){};

		///后门
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
	virtual unsigned int			Draw(){return 0;};//排序之后，透明OBJECT必须绘制自己！
	virtual void*					GetAllInside(int n){return 0;};
	//运动控制
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
	virtual void					SetLocationMatrix(const H3DMat4& mat){};//设置物体状态矩阵后，Z轴方向旋转角度自动清0
	virtual void					SetOrgMatrix(const H3DMat4& mat){};//设置物体原始状态矩阵后
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

	//层次关系控制，上升到IMoveObject
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
	///重新载入所有,返回1表示成功，0表示失败。
	virtual int						ReloadAll()	{return 1;};

	//参数：
	//0：等同于ReloadAll
	//1：Reload Geometry
	//2：Reload 材质（包括贴图）
	//3：Reload 贴图

	//返回值-1为失败，大于0的数表示reload了几张贴图

	virtual int						ReloadFromDisk(int reloadContent){return 0;};
	///获得本物体使用的显存
	virtual unsigned int GetVideoMemUse(){return 0;};

	///获得性能数据
	virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

	///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
	virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){};

	//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
	//返回值为1表示相交
	virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;};

	//设置颜色系数
	//f是颜色系数的数组，arraysize是颜色系数数组长度
	//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
	virtual int SetMaterialColorFactor(float* f,int arraysize){return 1;};

	/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
	/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
	///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
	/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
	virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;};

	//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
	virtual unsigned int			GetFlags(){return 0;};
	virtual void					SetFlags(unsigned int nFlags){};

	///设置材质lod,nLod{}即为最高细节，nLod=2为最简单细节
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

		///创建主窗口
		virtual int						CreateRenderWindow(const tWindowCreateInfoImp& info)
		{
			m_createInfo = info;
			m_hwnd = info.hWnd;  
			hpenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
			return 0;

		}
		///创建附加窗口
		virtual int						CreateAddionalRenderWindow(HWND hwnd,int w,int h){return 0;};

		///设置当前活动的渲染窗口的ID，返回值小于0表示失败。该函数的作用是多窗口渲染的时候指定接下来的绘制 是绘制到哪个窗口上
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

		///窗口缩放，最后一个参数是窗口的ID,默认的-1表示是主窗口
		virtual bool					ResizeWindowForEditor(int w,int h,int windowID=-1){return 0;};
		virtual bool					ResizeWidnowEx(int w,int h,bool windowed,bool topmost,bool popup,bool maxbox,bool pad_win_frame_size=true,int windowID=-1){return 0;};

		///sssa2000 08.04.03  !强迫设备lost，用于vista下
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
			//MoveToEx(hdc,25,25,NULL);//原来鼠标的位置
			//LineTo(hdc,100,300);//point是当前鼠标的位置
			//ReleaseDC(m_hwnd,hdc); 
		};
		virtual unsigned int			GetClearFlags(){return 0;};
		virtual void					SetClearFlags(unsigned int nFlags){};
		virtual void					ClearScreen(){};
		virtual void					SetClearColor(const float *col){};
		/// 设置场景环境光颜色
		virtual void					SetAmbientColor(const float *col){};
		/// 设置角色环境光颜色			
		virtual void					SetActorAmbientColor(const float *col){};
		virtual void					SetWireMode(bool bWire){};
		virtual void					UpdateCamera(){};
		//!创建物理场景
		virtual bool				    CreatePhyWorld(int& outId){return 0;};
		//!创建OBB
		virtual bool					CreateObbCollisionData(unsigned int name,IMoveObject* pModel,int phyId){return 0;};
		//!创建triangleMesh,参数是读取过后的碰撞点和索引
		virtual bool					CreateTriangleMesh(unsigned int name,const H3DVec3* points,const unsigned short* indexs,const H3DVec3* normals,int vertexNums,int triNums,int phyId){return 0;};
		//!加载ASE格式的triangleMesh。内存由外界分配.需要注意的是该接口需要调用2次,1次传出大小,1次填写数据.当传入的内存指针是0时填写大小
		virtual bool					LoadTriangleMesh(const char* aseName,int& vertexNums,int& triNums,H3DVec3* points,unsigned short* indexs,H3DVec3* normals){return 0;};
		//!释放碰撞数据
		virtual void					ReleaseCollisionData(unsigned int name,int phyId){};
		//!释放物理世界
		virtual void					ReleaseCollisionScene(int phyId) { };
		//!设置碰撞数据
		virtual void					SetCollisionData(const tCollisionObbInfo& obbInfo,int phyId){};
		//!更新物理引擎
		virtual void					UpdatePhx(int phyId,unsigned int interval){};//更新物理必须由外部进行
		//!老接口
		virtual void					UpdatePhx(unsigned int interval=0){};//更新物理必须由外部进行
		//!设置物理世界的属性,包括最小时间片,模拟时间(为负值表示取实际的渲染时间,为正值表示恒定模拟时间片,重力值
		virtual void					SetPhxPara(float fixedTime,float simTime,float gravity){};
		//!获取碰撞结果,传入一个物体ID,物理世界ID,返回和其发生碰撞的物体ID,类型,碰撞点,碰撞面,碰撞法线等信息
		virtual bool					GetCollisionResult(int phyId,unsigned int& collName0,int& collType0,unsigned int& collName1,int& collType1,tCollisionResultInfo& collInfo){return 0;};

		//!和某指定物体的射线查询.phxWorldId:物理世界id,objId:被查询的物体ID.返回1表示查询成功,0表示查询失败
		//目前只支持和桥梁类型的碰撞物体查询
		virtual int						RayTestSingObject(float x,float y,H3DVec3& collRes,unsigned int phxWorldId,unsigned int objId) {return 0;};

		//by sssa2000 08.04.08
		virtual void SetEnableAllPhy(bool b){};
		virtual bool GetEnableAllPhy(){return 0;};
		//by lzp,释放掉物理世界
		virtual void ReleasePhxScene(){};
		//获取切片数据,传入一段足够大的内存,返回实际填充的大小,idBufferSize表示数组元素个数
		virtual bool GetPhxWorldCopyInfo(int phyId,unsigned int* pIdBuffer,unsigned int idBufferSize,unsigned int& idRealSize,char* pBuffer,unsigned int bufferSize,unsigned int& realSize)const {return 0;};
		//设置切片数据,传入数据Buffer,buffer大小.idBufferSize表示数组元素个数
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
		//以下2个函数是为了辅助调试添加的，其中传入的指针必须是分配好空间的，且不小于8的数组,函数会把指定的值复制到数组中
		//成功后，数组中前4个为 near,后4个为far 平面
		virtual void					GetCullFrustumCornerPoint(H3DVec3* p){};
		virtual void					GetRenderFrustumCornerPoit(H3DVec3* p){};
		//---------------------------------------------------------
		virtual void					PushCamera(){};
		virtual void					PopCamera(){};
		virtual void					SetReflectCamera(const float* vec4Plane){};//以一个平面与当前CAMERA作为基准设置反射CAMERA
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

		//从外部文件载入一个地形
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
		//将中的参数设置给地形的全局光照
		virtual void					SetGlobalLight(ILight* pLight){};
		//得到影响地形的全局光接口
		virtual ILight*					GetGlobalLight(){return 0;};


		virtual IBillboard*				CreateBillboard(BILLBOARD_TYPE type,BILLBOARD_PROPERTY property){return 0;};



		//////////////////////////////////////////////////////////////////////////
		//Pipe Functions
		virtual void					PushModel(IModel* pModel){};
		//放入所有CBillboard,CUserObject
		virtual void					PushMoveObject(IMoveObject* pMoveObj){};
		virtual void					PushNewPipe(PIPE_TYPE type,unsigned int flags,ILight* pLight, PipeSettingFunc pStartFunc=0,PipeSettingFunc pEndFunc=0){};
		virtual void					ProcessCurrentPipe(){};//对其中物体进行排序等操作,所有管道都需要此操作
		virtual unsigned int			RenderPushedPipes(){return 0;};//绘制已经压入的所有PIPE 
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
		某些显卡只支持16bit的索引数据, 为了减少处理上的麻烦, 客户端的索引buffer统一使用unsigned short格式
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
		//返回0 是正常，否则是ERR CODE
		virtual int						IsThreadTaskOK(unsigned int task_id){return 0;};
		virtual int						GetThreadTask(int thread_id=0){return 0;};
		//返回0 是没有发生作用，1 成功把任务从序列中删除， 2 任务已经完成 3， 是任务已经完成，但是失败了
		virtual int						CancleTask(unsigned int task_id,int threadid=0){return 0;};
		virtual void					SetAsynMode(int n){};
		virtual int						GetAsynMode(){return 0;};
		virtual void					LockRenderer(){};
		virtual void					UnlockRenderer(){};
		virtual bool					IsRendererLocked(){return 0;};
		virtual void					MakeCurrent(){};
		virtual void					CleanFIFO(){};

		//==============================================

		//!得到当前关卡,无用
		virtual ILevel* GetLevel(void)const {return 0; };
		//!创建相机
		virtual ICamera* CreateCamera(void) 
		{
			return dynamic_cast<CCamera*>(EnginePool::m_camera_pool->createObj("camera")); 
		};
		//!设置渲染相机
		virtual void SetRenderCamera(ICamera* pCam) { };
		//!得到渲染相机
		virtual H3DI::ICamera* GetRenderCamera(void)const {static CCamera renderCamera; return &renderCamera; };
		//!设置culling相机
		virtual void SetCullingCamera(ICamera* pCam) { };
		//!得到culling相机
		virtual H3DI::ICamera* GetCullingCamera(void)const {static CCamera cullingCamera; return &cullingCamera; };
		//!画AABB
		virtual void DrawAABB(const float* aabb, const float* color, float lineWidth)const { };
		//!画线段
		virtual void DrawSegment(const H3DVec3& begin, const H3DVec3& end, const float* color, float lineWidth)const { };
		//!无用
		virtual void SetRenderMode(ERenderMode) { };

		//!创建粒子VB
		virtual IParticleVB* CreateParticleVB(void) {return 0; };
		//!push一个粒子进行渲染
		virtual void PushParticle(H3DI::IMoveObject* pObj) { };

		//!打开一个动作库
		virtual bool                    OpenActionLib(const char* fileName)const {return 0;};
		//!读取一个动作文件
		virtual bool                    LoadAction(const char* actName, ISkeletonModel* p)const {return 0;};
		virtual unsigned int			LoadActionAsyn(const char* actName, H3DI::ISkeletonModel* p){return 0;};
		//!全屏blur
		virtual void                    FullScreenBlur(unsigned int filterWidth, unsigned int filterPass, const unsigned int* viewPort = 0) { };
		//!开始渲染残像
		virtual void                    MotionBlurBegin(const float* pColor) { };
		//!残像渲染结束
		virtual void                    MotionBlurEnd(void) { };
		//!衰减残像
		virtual void                    DarkenMotionBlurMap(float rate) { };
		//!读取关卡文件
		virtual ILevel*                 LoadLevel(const char* fileName) {return 0; };
		virtual unsigned int			LoadLevelAsyn(const char* fileName,ILevel** ppLevel){return 0;};
		//!push一个关卡进行渲染
		virtual void                    PushLevel(ILevel* p) { };
		//!渲染关卡的阴影
		virtual void                    RenderLevelShadowMap(ILevel*) { };
		//!执行命令,可扩展接口
		virtual void					Exec(unsigned int code, const char* argument, unsigned int argumentLength, char* buffer, unsigned int bufferLength) { };
		//!查询接口
		virtual void*					QueryInfo(int QueryCode,void* param1,void* param2,void* param3,void* param4,void* param5){return 0;};
		//!设置是否使用动作库,IRender初始化的时候调用一次即可,默认为不使用
		virtual void					SetUseActionLib(bool val) { };
		//!是否使用动作库
		virtual bool					UseActionLib(void)const {return 0;};
		//!设置是否合并Actor的Bodypart贴图,IRender初始化的时候调用一次即可,默认为不使用
		virtual void                    SetMergBodyPart(bool val) { };
		//!是否合并bodypart
		virtual bool                    MergBodyPart(void)const {return 0;};
		//清空motionblur H3DMap的内容,一般在相机改变位置的时候调用
		virtual void                    ClearMotionBlur(void) { };
		//!设置SkeletonModel的实现是否使用多轨动画,默认为不使用
		virtual void                    SetUseActionChannel(bool val) { };
		//!SkeletonModel的实现是否使用多轨动画
		virtual bool                    UseActionChannel(void)const {return 0;};
		//!在动作库模式下,读取一组动作
		virtual void                    PreLoadActions(const char** actNames, unsigned int actCount, ISkeletonModel* pSKL) { };
		//!在动作库模式下,清空前面读进来的所有动作，如果bOnlyClearUnUsed为true，只会清空未使用的动作。
		virtual void                     ClearAllActions(bool bOnlyClearUnUsed = true) { };
		//!重新读取动作库中的一个动作(用原来的文件名)
		virtual bool                     ReloadAction(const char* actName, ISkeletonModel* pSKL) {return 0;};

		//!设置是否开启布料模拟
		virtual void                     SetUseCloth(bool val) { };
		//!是否开启布料模拟
		virtual bool                     UseCloth(void)const {return 0;};
		//!设置是否优先使用三角形列表
		virtual void                     SetUseTriList(bool val) { };
		//!是否优先使用三角形列表
		virtual bool                     GetUseTriList(void)const {return 0;};


		//!设置贴图缩放参数
		virtual void                     SetImageScaleFactor(unsigned int) { };
		//!得到贴图缩放参数
		virtual unsigned int          GetImageScaleFactor(void)const {return 0; };
		//!清空无用的贴图,shader等资源
		virtual unsigned int          ClearMaterialLib(void) 
		{
			EnginePool::clearMaterailLib();
			EnginePool::dump();
			return 0; 
		};
		//!得到状态监视器
		virtual const IEngineMonitor*    GetEngineMonitor(void)const { return 0;}; 
		//!设置是否优先读取二进制
		virtual void					  SetFavorBinary(bool val) { };
		//!是否优先读取二进制
		virtual bool                      FavorBinary(void)const {return 0;};
		//!得到运行时模式
		virtual ERuntimeMode       GetRuntimeMode(void)const { return RUNTIME_DX9; };
		//!push世界矩阵
		virtual void					  PushViewMatrix(void)const { };
		//!pop世界矩阵
		virtual void                      PopViewMatrix(void)const { };
		//!push投影矩阵
		virtual void                      PushProjectionMatrix(void)const { };
		//!pop投影矩阵
		virtual void                      PopProjectionMatrix(void)const { };
		//!得到渲染状态管理器
		virtual const IRenderStateManager* GetRenderStateManager(void)const {
			static CRenderStateManager renderStatusMng;
			return &renderStatusMng;};
			//!创建一张贴图
			/**
			client创建了texture或者RT后,要执行addref操作,否则在清理内存的时候可能会把这个贴图删掉
			销毁前,要先执行delref,再执行release
			以下第一个函数是创建一个指定大小的texture，第二个是通过磁盘文件创建一个texture,第二个参数表示mipmap的等级 0表示自动生成
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
			//!创建一个RT
			virtual IRenderTarget*      CreateRenderTarget(unsigned int& width, unsigned int& height, ETextureFormat fmt, ERenderTargetType type) 
			{
				CRenderTarget* rt = dynamic_cast<CRenderTarget*>(EnginePool::m_rendertarget_pool->createObj("render_target"));
				alwaysAssert(rt);
				rt->setWH(width, height);
				return rt; 
			};
			//!设置是否将贴图压缩成DXT格式
			virtual void                      SetCompressTexture(bool res)const { };
			//!是否将贴图压缩成DXT格式
			virtual bool                      CompressTexture(void)const {return 0;};
			//!得到平行投影矩阵
			virtual void                       GetOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far, H3DMat4& out)const { };
			//!修改当前view矩阵
			virtual void                       TranslateViewMatrix(float x, float y, float z)const { };
			//!旋转当前view矩阵
			virtual void                       RotateViewMatrix(float degree, float x, float y, float z)const { };
			//!得到屏幕坐标系下的坐标
			virtual void                       GetScreenPos(const H3DVec3& objPos, const H3DMat4& worldViewMat, const H3DMat4& projMat, 
				unsigned int x, unsigned int y, unsigned int width, unsigned height, 
				float minZ, float maxZ, H3DVec3& screenPos)const { };
			//!设置贴图参数
			virtual void                        SetTextureParams(bool scale, bool compress, bool mipmap)const { };
			//!得到贴图参数
			virtual void                        GetTextureParams(bool& scale, bool& compress, bool& mipmap)const { };
			//!设置全屏、窗口模式
			virtual void						ToWindowedMode(){};
			virtual void						ToFullScreenMode(){};
			//!设置全屏抗锯齿模式
			virtual void                         SetAAMode(EAAMode aa) { };
			//!得到全屏抗锯齿模式
			virtual EAAMode              GetAAMode(void)const {return AA_Disable; };
			//!得到支持的抗锯齿类型
			virtual unsigned int           GetSupportedAAMode(void)const {return 0; };
			//!设置各项异性过滤模式
			virtual void                          SetAFMode(EAFMode) { };
			//!得到各项异性过滤模式
			virtual EAFMode               GetAFMode(void)const {return AF_Disable; };
			//!得到支持的各项异性过滤模式
			virtual unsigned int           GetSupportedAFMode(void)const { return AF_Disable;};
			//!设置是否开启垂直同步
			virtual void                          SetVSync(bool val) { };
			//!是否开启垂直同步
			virtual bool                          GetVSync(void)const {return 0;};
			//!设置阴影模式
			virtual void                          SetShadowQuality(EShadowQuality q) { };
			//!得到阴影模式
			virtual EShadowQuality    GetShadowQuality(void)const { return Shadow_Off;};
			//!设置是否开启布料
			virtual void                          EnableCloth(bool val) { };
			//!是否开启布料
			virtual bool                          ClothEnabled(void)const { return 0;};
			//!更新显示设置
			virtual void                         ApplyVideoSettings(void) { };
			virtual void						 ApplyVideoSettings2(void) { };
			//!得到当前的分辨率
			virtual void						 GetCurrentReslution(int& w,int& h){};
			//!卸载所有bp3贴图
			virtual void                         UnloadBodyPartImages(void) { };
			virtual unsigned int                 UnloadBodyPartImagesAsyn(void) {return 0; };
			//!得到profile数据
			virtual void                         GetPerfData(tPerfData& data)const { };
			//!设置重力
			virtual void                         SetGravity(const H3DVec3& g) { };
			//!得到重力
			virtual const H3DVec3&      GetGravity(void)const {  static H3DVec3 v3;v3.Set(0,0,0); return v3;};
			//!卸载单一一个bp3贴图
			virtual void                         UnloadSingleBodyPartImages(const char* bpName,int ActorStamp) { };

			//!设置事件处理器
			virtual void                          SetEventHandler(IEngineEventHandler* pHandler) { };
			//!得到事件处理器
			virtual IEngineEventHandler* GetEventHandler(void)const { return 0; };

			//!抓屏
			virtual bool SaveScreenToFile(const char* ){return 0;};
			//创建IImage
			virtual IImage* GetImageInterface(void){return 0;};

			virtual IPerfMonitor* GetPerfMonitor(){return 0;};

			//by sssa2000 090330
			//为编辑器添加的新接口
			virtual void beginWireFrameDraw(){};
			virtual void endWireFrameDraw(){};
			virtual void drawPhy(){};
			//add by liren
			//将地形放入管线
			virtual void PushTerrain(H3DI::ITerrain *pTerrain) { };
			//!add by liren

			//add by liren
			//在地形上创建一个面片
			virtual	IModel*			GenerateSmallPatch(float length,float width,ITerrain *pTerrain) {return 0; };
			//!add by liren
			//硬件选择
			//开启硬件选择，传入一个屏幕矩形，和选择方式,
			virtual	bool			BeginHardwareSelect(const int *pRect,bool bSelectFirst, int WindowID) {return 0; };
			//传入一物体和它相对的ID
			virtual void			PushHardwareSelectObject(IMoveObject *pobject,unsigned int nId) { };
			//处理物体的选择
			virtual	void			ProcessMoveObjectSelect() { };
			//!压入一个Id，适用于自定义顶点，线和VB等
			virtual void			PushSelectID(int ID) { };
			//!无用
			virtual int				GetSelectID(void)const {return 0; };
			//结束选择，返回结果
			virtual	int				EndHardwareSelect(unsigned int *pRetIds,int num) { return 0;};
			//!设置镜头光晕参数
			virtual void			SetLensParameters(const tLensEnv& lenEnv,const tLensFlareParameters* lensPara,unsigned int nums) { };
			//!设置太阳高度,默认支持一个太阳
			virtual void			SetSunHeight(float height){};
			//!设置太阳位置,编辑器用
			virtual void			SetSunPos(const H3DVec3& pos){};
			//!获取太阳位置,编辑器debug用
			virtual void			GetSunPos(H3DVec3& pos){};

			//雾(新渲染系统)
			virtual H3DI::IFog*					CreateFog() {return 0; };
			//设置全局雾（目前全局雾不需要创建）
			virtual void SetGlobalFog(float r,float g,float b,float density,float start,float fogmin){};
			//重新载入贴图
			virtual int ReloadTexture(const char* filename){return 0;};

			//后处理相关接口
			//添加删除后处理效果的接口
			virtual bool					AddPostProcess(const char *FileName) {return 0;};

			virtual bool					DelPostProcess(const char *FileName) {return 0;};
			//开启
			virtual void					EnablePostProcess(bool bEnable) { };
			//设置参数,后处理特效供外界可调的参数,用这个接口设置。
			//具体有哪些参数，每个参数什么意义，由相关文档提供。
			virtual void					SetPostProcessParams(const char* name,const void *pParmeValue) { };

			//设置后处理影响的窗口。
			virtual void					SetPostProcessWindowId(int){};
			//获得后处理影响的窗口。
			virtual int						GetPostProcessWindowId(){return 0;};



			//设置世外场景的全局环境
			virtual void SetOutDoorEnvInfo(const OutDoorEnvInfo& info){};
			virtual void GetOutDoorEnvInfo(OutDoorEnvInfo* info){};

			//获得运行时信息
			virtual void GetRuntimeInfo(IRuntimeInfo* pRes){};

			//将焦平面距离，景深范围，最大弥散圆半径设置给引擎的摄相机
			virtual void SetFocalParam(float fFocalplane,float fDOFRange,float fConfusion) { };
			//得到焦平面距离，景深范围，最大弥散圆半径设置给引擎的摄相机
			virtual void GetFocalParam(float &fFocalplane,float &fDOFRange,float &fConfusion) { };


			//客户端有PushCamera和PopCamera需求，以下两个函数分别将景深参数压栈或弹出栈。
			virtual void PushFocalParam() { };
			virtual void PopFocalParam() { };
			//将一个模型添加到骨骼模型实例包
			virtual	void AddSkeletonModelToInstancing(H3DI::ISkeletonModel *pSkeletonModel) { };
			//处理模型添加到骨骼模型实例包
			virtual void ProcessSkinedInstancing() { };
			//得到场景深度图
			virtual IRenderTarget* GetSceneDepthUsedInPostprocess() {return 0; };

			//设置是否开启延迟光照绘制
			virtual void EnableLightPrePassRendering(bool b){};

			//给出一个材质实例文件名 以及材质球的名字得到INewShader
			virtual H3DI::INewShader* GetNewShader(const char* filename,const char* matName)
			{
				std::string sn = filename;
				sn += "@";
				sn += matName;

				return dynamic_cast<H3DI::INewShader*>(EnginePool::m_shader_pool->createObj(sn.c_str()));
			};

			//得到错误代码
			virtual int GetEngineLastError(){return 0;};

			//得到错误代码的描述
			virtual const char* GetErrorDesc(int errCode){return 0;};

			//添加一个高亮模型，高亮模型每帧都必须设置，如果不设置就没有模型被高亮
			virtual int AddHighLightModel(const H3DI::IModel* pModel){return 0;};

			virtual unsigned int GetRenderFrameCount() { return 0;};

			//设置次表面散射颜色
			virtual void		SetTransmissionColor(const float* col) { };

			//可以设置景深参数的后处理设置参方式
			virtual void					SetPostProcessParams1(const char* name,const void *pParmeValue) { };

			///设置引擎使用者
			virtual void					SetUser(EngineUserDefine u){};
			//开始DrawIndex
			virtual void					BegineDrawIndex() { };

			//结束DrawIndex
			virtual void					EndDrawIndex() { };

			///开始查询本帧显存用量
			virtual void BeginCurrentFrameVideoMemQuery(){};

			///结束查询本帧显存用量
			virtual unsigned int EndCurrentFrameVideoMemQuery(){return 0;};

			///设置是否开启2d绘制。
			virtual void Set2DMode(bool b){};

			///push 2d物体
			virtual void Push2DObj(IMoveObject* pMoveObj){};

			///Render 2d物体
			virtual void Render2DObj(){};
			//创建一个后处理对象
			virtual IPostProcessEffect*		CreatePostProcessEffect(){return 0;};
			//释放一个后处理对象
			virtual void					ReleasePostProcessEffect(IPostProcessEffect *pIEffect){};

			//得到默认的后处理对象
			virtual IPostProcessEffect*		GetDefaultPostProcessEffect(){return 0;};

			//得到当前的后处理对象
			virtual IPostProcessEffect*		GetCurrentPostProcessEffect(){return 0;};

			//设置当前的后处理对象
			virtual void					SetCurrentPostProcessEffect(IPostProcessEffect *pIEffect){};
			//得到当前地形
			virtual ITerrain*				GetActiveTerrain() {return 0; };
			//获得与光源绑定的体积光是否开启的标志
			virtual	bool					GetLightShaftEnable() {return 0; };
			//设置与光源绑定的体积光是否开启的标志
			virtual	void					SetLightShaftEnable(bool bEnable) { };
			//// !使用新材质系统Draw Index
			virtual void					DrawIndexNew(PRIMITIVE_TYPE nTriangleType,unsigned int nElemNum,unsigned int nVertNum,unsigned int nNormalOffset,unsigned int nTexcoordOffset,unsigned int nColorOffset,unsigned int nTexcoordOffset2)
			{
				if(nTriangleType == H3DI::TRIANGLE_STRIP)
				{

				}
			};
			///! 命令行
			virtual int						OnCommandLine(const char* commandLine) {return 0; };
			//使用硬件选择
			virtual void					SetUseHardWareSelection(bool buse) { };
			//获取是否使用硬件选择
			virtual bool					UseHardWareSelection() {return 0;};

			//获取全局的性能数据
			virtual void* GetGlobalPerfData(GLOBAL_PERFDATA_TYPE t){return 0;};

			/// 开关材质的动画效果
			virtual void EnableMaterialAnimation(bool b) { };

			// 设置当前渲染模式（普通、线框、普通+线框）
			virtual void					SetFillMode(EFILLMODE mode) { };
			// 设置线框的颜色
			virtual void					SetWireframeColor(const float* color)  { };

			/// 适应新材质INewShader的需求/// 设置DrawIndex需要使用的INewShader
			virtual void					SetNewShader(H3DI::INewShader* pShader) { };

			///开始设置当前绘制要使用的灯光
			virtual void BeginSetCurrentLight(){};

			//设置当前绘制要使用的灯光
			//t：场景光还是人物光
			//pLight：光源指针
			//返回值：如果返回-1表示数量超过规定，设置不成功。返回大于0的值表示成功
			virtual int SetCurrentLight(H3DI::IPrePassLight* pLight){return 0;};

			///结束设置当前绘制要使用的灯光
			virtual void EndSetCurrentLight(){};

			///获取当前绘制要使用的灯光的数量
			virtual int GetCurrentLightNum(H3DI::LightAffectParam t){return 0;};

			//开始向ViewPort中画东西
			virtual void BeginViewPort() { };
			//结束向ViewPort中画东西
			virtual void EndViewPort() { };

			//打开全局的参数库
			virtual int OpenPhxParaLib(const char** paraLibs,int bufferSize,int phxParaType) { return 0;};
			//获取指定名称的物理参数
			virtual int GetPhxParaFromLib(const char* pName,void** paraData,int bufferSize,int phxParaType) {return 0; };
			//将指定名称的物理参数加入到参数库
			//opType: 0,不覆盖 1,覆盖。返回0表示失败。返回1表示成功.有同名的需要提示是否覆盖
			virtual int AddPhxParaToLib(const char* pName,void** paraData,int bufferSize,int phxParaType,int opType) {return 0; };

			//获取鼠标点选时的射线方向,使用当前的渲染相机的参数
			//point和dir都是x、y、z三个分量
			virtual int GetMouseRayDir(int mouse_x,int mouse_y, H3DVec3* pos, H3DVec3* dir){return 0;};

			virtual int GetMouseRayDirEx(const H3DVec3& eye_position, const H3DVec3& camera_direction, const H3DVec3& camera_up, float f_near,
				float f_far, unsigned int screen_width, unsigned int screen_height, float fovX, int mouse_x,int mouse_y, H3DVec3* pos, H3DVec3* dir){return 0;};

			//读入包围体文件，返回值1表示成功，小于0不成功
			virtual int LoadCustomBoundingVolume (const char* filename){return 0;};

			//!生成一个fxinfo
			virtual IFxInfo* CreateAFxInfo(){return 0;};
			//创建一个ILevel
			virtual	ILevel* CreateLevel(const char * name) 
			{
				return dynamic_cast<ILevel*>(EnginePool::m_level_pool->createObj(name, name));
			};

			// 将当前帧渲染到贴图
			// origPathName: 原贴图的路径名,用于获得格式，大小
			virtual bool RenderToTexture(const char* origPathName, const char* pathName) {return 0;};
	protected:
		SpecialEffectManager m_specialEffectMng;
	};

}
#endif