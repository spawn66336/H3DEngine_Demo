#ifndef MODELSHELL
#define MODELSHELL
namespace H3DI
{
	class CAnimationChannel : public IAnimationChannel
	{
	public:

		H3DVec3 v;
		CAnimationChannel()
		{
			v.Zero();
		}

		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		/************************************************************************/
		/* 0 : succeed
		1 : action is not in actionlib
		2 : action is in actionlib, but can't load ase file
		3 : Invalid Params
		*/
		/************************************************************************/
		/**
		设置单一静态帧
		*/
		virtual int SetStaticFrame(const char* name) {return 0;}
		/**
		设置静态帧
		*/
		virtual int SetStaticFrame(unsigned int id, const char* name, float weight) {return 0;}
		/**
		设置静态帧序列
		*/
		virtual void SetStaticSeq(const char** names, const float* intervals, unsigned int count, bool loop) {}
		/**
		设置动画帧序列
		*/
		virtual void SetActionSeq(const char** names, unsigned int count, bool loop) {}
		/**
		设置单一动画帧
		*/
		virtual int SetAction(const char* names, bool loop) {return 0;}
		virtual unsigned int SetActionAsyn(const char* names, bool loop){return 0;}

		virtual int SetNextAction(const char* names, bool loop) {return 0;}
		/**
		得到当前动作
		*/
		virtual IAction* GetCurrAction(void) {return 0;}
		/**
		Clear all stuff
		*/
		virtual void Clear() {}
		/**
		设置这个通道的权重, 所有激活通道的权重会被规一化
		*/
		virtual void SetWeight(float) {}
		/**
		得到权重
		*/
		virtual float GetWeight(void)const {return 0;.5f;}
		/**
		设置动作过渡时间
		*/
		virtual void SetTransitionTimeFromCurrent(float) {}

		virtual void SetTransitionTimeToNext(float) {}
		/**
		得到动作过渡时间
		*/
		virtual float GetTransitionTimeFromCurrent(void)const {return 0.5;}

		virtual float GetTransitionTimeToNext(void)const {return 0.5;}
		/**
		设置当前动作时间,只在静帧序列,单一动画帧,动画帧序列模式下起作用
		*/
		virtual void SetCurrActionTime(float) {}
		/**
		得到当前动作时间
		*/
		virtual float GetCurrActionTime(void)const {return 0.5;}
		/**
		得到当前动作长度
		*/
		virtual float GetCurrActionLength(void)const {return 0.5;}
		/**
		只否正在过渡动作
		*/
		virtual bool IsTransitingToNext(void)const {return true;}

		virtual bool IsTransitingFromCurrent(void)const {return true;}
		/**
		是否循环
		*/
		virtual bool IsLoop(void)const {return false;}
		/**
		是否独占
		*/
		virtual bool IsDominating(void)const {return false;}
		/**
		得到channel ID
		*/
		virtual unsigned int GetChannelID(void)const {return 0;}
		/**
		设置动作过渡模式, 默认为TRANSITION_SYNC
		*/
		virtual void SetTransitionMode(int mode) {}
		/**
		得到动作过渡模式
		*/
		virtual int GetTransitionMode(void)const {return 0;}

		virtual void Draw(void)const {}

		//得到当前动作的名字，返回值：0表示没有当前动作，1：表示成功，-1表示传入的buffer长度太小
		virtual int GetCurrentActionName(char* buffer,unsigned int bufferlen){strcpy(buffer, "testname"); return 1;}

		//得到下一个动作的名字，返回值：0表示没有next动作，1：表示成功，-1表示传入的buffer长度太小
		virtual int GetNextActionName(char* buffer,unsigned int bufferlen){strcpy(buffer, "testname");return 1;}

		//得到下一个动作是否循环，返回-1表示没有下一个动作 返回0表示不循环 返回1表示循环
		virtual int GetIsNextActionLoop(){ return -1;}
		//在当前动作播放设置循环参数
		virtual	void SetActionLoop(bool) {}
		//在当前下一个播放设置循环参数
		virtual void SetNextActionLoop(bool){}
		//动作过渡时的插值方式
		virtual void SetTrainsitionMethod(EActionTransitionMethod) {}

		virtual void SetRootPos(const H3DVec3 &){}
		virtual const H3DVec3& GetRootPos(void) const { return v;}

		virtual ~CAnimationChannel(){}
	};

	class CAnimation : public IAnimation
	{
	public:

		H3DVec3 m_dummy_vec;
		H3DQuat m_dummy_quat;


		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual	void					clear(){}

		virtual const H3DVec3&	get_pos(void)const {return m_dummy_vec;}
		virtual const H3DQuat&	get_rot(void)const {return m_dummy_quat;}

		virtual void					update(float interval){}
		virtual void					set_passtime(float *time_passed){}
		virtual int						getrotnum()const{return 0;}
		virtual int						getposnum()const{return 0;}


		virtual void					getposkey(int i, H3DVec3& pos){}
		virtual void					getrotkey(int i, H3DQuat& rot){}

		virtual float					get_animtime()const{return 0;}
		virtual float					get_curtime()const{return 0;}
		virtual int						get_curposfrm()const{return 0;}
		virtual int						get_currotfrm()const{return 0;}
		virtual void					reset(){}
		virtual void					setloop(int lp){}
	};

	class CAction : public IAction
	{
		CAnimation m_ani;
		CAnimationChannel m_ani_channel;
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual		~CAction(){};
		virtual const char*				GetName(){return "";};
		virtual IAnimation*				GetAnimation(unsigned int nBone){return &m_ani;};
		virtual float					GetAnimationTime(){return 10;};
		virtual void					SetAnimationRunningTime(float fAnimTime){};
	};

	class CBone: public IBone
	{
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual const char*				GetName(){return "root";}
		virtual	int						GetID()const{return 12;}
		virtual IBone*					GetParent(){return 0;}
		virtual unsigned int			GetNumChildren(){return 0;}
		virtual IBone*					GetChild(unsigned int n){return this;}
	};

	class CSkeleton: public ISkeleton
	{
		CBone m_bone;
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual IBone*					GetRootBone(){return &m_bone;}
		virtual unsigned int			GetNumBone(){return 1;}
		virtual IBone*					GetBone(unsigned int n){return &m_bone;}
		virtual IBone*					GetBone(const char* sBoneName){return &m_bone;}
	};

	class CModel: public IModel
	{ 
		int m_flag;
	public:
		CModel():m_flag(0)
		{
		}
		virtual ~CModel(){};
		virtual void Release()
		{
			EnginePool::m_model_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}


//IMODEL
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//运行控制

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//可视范围
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//获得一个模型号的顶点和索引数据，
		//pVertBuf，pIndexBuff，顶点和索引的指针，用于传出数值
		//pVertNum,pIndexNum顶点和索引的个数，当前pVertBuf，pIndexBuff都传入空指针是，返回顶点和索引的个数。一般情况下传入顶点和索引的个数。
		//返回值：当传入的顶点和索引的数量大于等于实际的模型所需的数量时返回真,否则返回假
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///重新载入这个模型的材质,返回1表示成功，0表示失败。
		virtual int ReloadMaterialInstance(){return 0;}
		///重新载入fx
		virtual int ReloadFx(){return 0;}

		///设置模型的模糊因子。0.0-1.0的浮点数。为0时表示模型完全清晰，为1是表示完全参与运动模糊。0-1之间的数表示参与运动模糊的程度。
		virtual void SetMotionBlurFactor(float fFactor) {}
		///设置遮挡查询的状态
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///获取是否使用遮挡查询的状态
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///获得查询结果，0表示被完全遮挡了
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//材质相关

		//高亮参数是2个颜色c0,c1，最终的颜色是c0*物体颜色+c1
		//返回值1表示成功，0表示失败。失败的原因是物体的材质不支持该操作
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///得到新材质，参数是网格的id，从0开始
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///得到新材质，参数是网格的名字
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///设置新材质 将影响到整个模型的所有mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给整个模型，将影响到整个模型的所有mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///从内存设置一个材质模板给模型，模型的所有网格都受到影响。返回1表示成功
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到改名字的网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//以下接口不要使用

		///设置物体是否具有反射效果
		///参数b:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///获取物体是否具有反射效果
		///返回:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///设置物体是否参与反射渲染
		virtual void SetCanBeReflection(bool b){}

		///获取物体是否参与反射渲染
		virtual bool GetCanBeReflection(){ return false;}

		///设置反射远平面
		///参数：远平面距离
		virtual void SetReflectionFarClipPlane(float dis){}

		///设置物体是否是折射物体
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///设置折射率
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//编辑器生成光芒时调用的接口
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//获取光芒文件数据
		//	返回：光芒数据值针
		//	参数：psize，输出型参数，返回光芒数据大小。
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//功能：计算一条射线与模型上的包围盒相碰撞的信息
		//参数
		//rayOrigin：传入三个浮点数组成的数组，表示一条射线的起始点
		//rayDir：传入三个浮点数组成的数组，表示一条射线的方向
		//ModelBVTestInfo ：一个数组首地址，用于结果的返回。所有碰撞到的包围盒按先后顺序将ID和碰撞点坐标返回给客户端。数组的大小不能小于一个模型的包围盒个数，包围盒个数由引擎再提供一个接中得到
		//nResultBufferCount：数组的元素个数，防止引擎填充数据越界
		//返回值：返回的结果的个数,如果为，说明没有碰撞
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//获取包围体的个数
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//功能：射线求交
		//	//参数
		//	RayOrg：传入的射线起点
		//	RayDir：传入射线方向
		//	fOutDistance：引擎返回的结果，这个数值是碰撞点到射线起点的距离。
		//返回值：射线与模型是否碰撞
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!挂饰相关接口
		//根据射线得到挂接数据集
		//第二个参数表示是否需要转换射线空间
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!挂接挂饰.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!取消挂接
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!预生成fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!设置LightMap贴图文件名及Info等,MeshID为此模型的网格索引
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh对应的lightmap size，与geometry中mesh的数量一致
		uvTexturePathName: 用于预览uv拆分的结果，如传入"d:\\lightmapUVTest%d.bmp",则
		保存出d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp等文件。
		不用时传NULL
		bReadOnlyModified: 用于返回是否修改了lmm文件的只读属性，可以为NULL
		返回值：成功返回true，失败返回false。如果失败，或者用更大的lightmapsize，或者让美术手动展uv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///后门函数
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }


	};

	class CMoveObject : public IMoveObject//IUserObject
	{
	public:
		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}
	};

	class CSkeletonModel : public ISkeletonModel
	{
	public:

		CSkeletonModel():m_flag(0)
		{
		}

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}


		int m_flag;
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//运行控制

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//可视范围
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//获得一个模型号的顶点和索引数据，
		//pVertBuf，pIndexBuff，顶点和索引的指针，用于传出数值
		//pVertNum,pIndexNum顶点和索引的个数，当前pVertBuf，pIndexBuff都传入空指针是，返回顶点和索引的个数。一般情况下传入顶点和索引的个数。
		//返回值：当传入的顶点和索引的数量大于等于实际的模型所需的数量时返回真,否则返回假
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///重新载入这个模型的材质,返回1表示成功，0表示失败。
		virtual int ReloadMaterialInstance(){return 0;}
		///重新载入fx
		virtual int ReloadFx(){return 0;}

		///设置模型的模糊因子。0.0-1.0的浮点数。为0时表示模型完全清晰，为1是表示完全参与运动模糊。0-1之间的数表示参与运动模糊的程度。
		virtual void SetMotionBlurFactor(float fFactor) {}
		///设置遮挡查询的状态
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///获取是否使用遮挡查询的状态
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///获得查询结果，0表示被完全遮挡了
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//材质相关

		//高亮参数是2个颜色c0,c1，最终的颜色是c0*物体颜色+c1
		//返回值1表示成功，0表示失败。失败的原因是物体的材质不支持该操作
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///得到新材质，参数是网格的id，从0开始
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///得到新材质，参数是网格的名字
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///设置新材质 将影响到整个模型的所有mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给整个模型，将影响到整个模型的所有mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///从内存设置一个材质模板给模型，模型的所有网格都受到影响。返回1表示成功
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到改名字的网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//以下接口不要使用

		///设置物体是否具有反射效果
		///参数b:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///获取物体是否具有反射效果
		///返回:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///设置物体是否参与反射渲染
		virtual void SetCanBeReflection(bool b){}

		///获取物体是否参与反射渲染
		virtual bool GetCanBeReflection(){ return false;}

		///设置反射远平面
		///参数：远平面距离
		virtual void SetReflectionFarClipPlane(float dis){}

		///设置物体是否是折射物体
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///设置折射率
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//编辑器生成光芒时调用的接口
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//获取光芒文件数据
		//	返回：光芒数据值针
		//	参数：psize，输出型参数，返回光芒数据大小。
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//功能：计算一条射线与模型上的包围盒相碰撞的信息
		//参数
		//rayOrigin：传入三个浮点数组成的数组，表示一条射线的起始点
		//rayDir：传入三个浮点数组成的数组，表示一条射线的方向
		//ModelBVTestInfo ：一个数组首地址，用于结果的返回。所有碰撞到的包围盒按先后顺序将ID和碰撞点坐标返回给客户端。数组的大小不能小于一个模型的包围盒个数，包围盒个数由引擎再提供一个接中得到
		//nResultBufferCount：数组的元素个数，防止引擎填充数据越界
		//返回值：返回的结果的个数,如果为，说明没有碰撞
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//获取包围体的个数
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//功能：射线求交
		//	//参数
		//	RayOrg：传入的射线起点
		//	RayDir：传入射线方向
		//	fOutDistance：引擎返回的结果，这个数值是碰撞点到射线起点的距离。
		//返回值：射线与模型是否碰撞
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!挂饰相关接口
		//根据射线得到挂接数据集
		//第二个参数表示是否需要转换射线空间
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!挂接挂饰.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!取消挂接
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!预生成fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!设置LightMap贴图文件名及Info等,MeshID为此模型的网格索引
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh对应的lightmap size，与geometry中mesh的数量一致
		uvTexturePathName: 用于预览uv拆分的结果，如传入"d:\\lightmapUVTest%d.bmp",则
		保存出d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp等文件。
		不用时传NULL
		bReadOnlyModified: 用于返回是否修改了lmm文件的只读属性，可以为NULL
		返回值：成功返回true，失败返回false。如果失败，或者用更大的lightmapsize，或者让美术手动展uv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///后门函数
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;


		virtual void					Release()
		{
			EnginePool::m_skeleton_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//骨骼渲染矩阵。和逻辑矩阵可能有所不同
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // 这个没有写完
		virtual const char*					GetActionName(unsigned int n){return "testname";} // 这个没有写完
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		得到某一个通道的动画, id合法值为[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // 这个没有写完，需要继续填写
		/*
		清除所有通道的数据
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		设置哪些动作通道为独占通道, buff=NULL会把所有通道设为非独占通道
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 为编辑器添加接口
		//绘制骨骼
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//关联两个物体的动作，使当前物体的动作根骨骼基于骑乘物体某个骨骼,iBoneId是绑定的骨骼ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//在更新骨骼模型的时候,是否对各个骨头进行计算, 默认值为true
		//如果该选项为false, 在ISkeletonModel的copy函数内也不需要拷贝骨头数据.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//设置坐骑矩阵,太特殊了,用于修正马对布料更新的影响
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//获得人物物理成员的名字,传入指定的物理实体类型
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//设置物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理参数,传入物理参数名,目前只支持布料
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!从参数库中删除掉一个参数,目前只支持布料
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//新建一个物理实体
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//删除一个物理实体
		virtual void   DelPhxEntity(const char* pName) {}
		//保存物理数据模板,类型由EPhxDataType指定
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//保存物理数据到bp3中,当前无用
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//加载.传入clearCur表明是否需要清楚掉当前已经读取的参数
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//获取当前所有布料参数组
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//融合物理结果
		virtual void   BlendPhxResult() {}
		//绘制物理
		virtual void   RenderPhxInfo() {}

		//填加一个附加在骨骼上的四元数，在动作计算之后，将旋转附加到骨骼上
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//清空所有的四元数
		virtual void	ClearAdditionalQuat() {}
		//获取布料光芒文件数据
		//返回：光芒数据值针
		//参数：psize，输出型参数，返回光芒数据大小。
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//缩放骨骼,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//动态设置该人物的物理
		virtual void                SetThisActorPhy(bool b){}
		//获得该人物的物理状态，注意，在人物一创建 就需要显示的设置一次SetThisActorPhy，这样每次的GetThisActorPhy才能生效
		virtual bool				GetThisActorPhy(){return false;}

		// 给某个mesh设置排序补偿值
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// 使某个mesh高亮显示
		// bHightLight true : 设置高光效果，false : 恢复
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//更新蒙皮到预定义姿态
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}
	};

	class CActor : public IActor
	{
	public:

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		void EnableHeelOffset(bool) {}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//运行控制

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//可视范围
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//获得一个模型号的顶点和索引数据，
		//pVertBuf，pIndexBuff，顶点和索引的指针，用于传出数值
		//pVertNum,pIndexNum顶点和索引的个数，当前pVertBuf，pIndexBuff都传入空指针是，返回顶点和索引的个数。一般情况下传入顶点和索引的个数。
		//返回值：当传入的顶点和索引的数量大于等于实际的模型所需的数量时返回真,否则返回假
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///重新载入这个模型的材质,返回1表示成功，0表示失败。
		virtual int ReloadMaterialInstance(){return 0;}
		///重新载入fx
		virtual int ReloadFx(){return 0;}

		///设置模型的模糊因子。0.0-1.0的浮点数。为0时表示模型完全清晰，为1是表示完全参与运动模糊。0-1之间的数表示参与运动模糊的程度。
		virtual void SetMotionBlurFactor(float fFactor) {}
		///设置遮挡查询的状态
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///获取是否使用遮挡查询的状态
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///获得查询结果，0表示被完全遮挡了
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//材质相关

		//高亮参数是2个颜色c0,c1，最终的颜色是c0*物体颜色+c1
		//返回值1表示成功，0表示失败。失败的原因是物体的材质不支持该操作
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///得到新材质，参数是网格的id，从0开始
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///得到新材质，参数是网格的名字
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///设置新材质 将影响到整个模型的所有mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给整个模型，将影响到整个模型的所有mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///从内存设置一个材质模板给模型，模型的所有网格都受到影响。返回1表示成功
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}


		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//以下接口不要使用

		///设置物体是否具有反射效果
		///参数b:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///获取物体是否具有反射效果
		///返回:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///设置物体是否参与反射渲染
		virtual void SetCanBeReflection(bool b){}

		///获取物体是否参与反射渲染
		virtual bool GetCanBeReflection(){ return false;}

		///设置反射远平面
		///参数：远平面距离
		virtual void SetReflectionFarClipPlane(float dis){}

		///设置物体是否是折射物体
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///设置折射率
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//编辑器生成光芒时调用的接口
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//获取光芒文件数据
		//	返回：光芒数据值针
		//	参数：psize，输出型参数，返回光芒数据大小。
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//功能：计算一条射线与模型上的包围盒相碰撞的信息
		//参数
		//rayOrigin：传入三个浮点数组成的数组，表示一条射线的起始点
		//rayDir：传入三个浮点数组成的数组，表示一条射线的方向
		//ModelBVTestInfo ：一个数组首地址，用于结果的返回。所有碰撞到的包围盒按先后顺序将ID和碰撞点坐标返回给客户端。数组的大小不能小于一个模型的包围盒个数，包围盒个数由引擎再提供一个接中得到
		//nResultBufferCount：数组的元素个数，防止引擎填充数据越界
		//返回值：返回的结果的个数,如果为，说明没有碰撞
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//获取包围体的个数
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//功能：射线求交
		//	//参数
		//	RayOrg：传入的射线起点
		//	RayDir：传入射线方向
		//	fOutDistance：引擎返回的结果，这个数值是碰撞点到射线起点的距离。
		//返回值：射线与模型是否碰撞
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!挂饰相关接口
		//根据射线得到挂接数据集
		//第二个参数表示是否需要转换射线空间
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!挂接挂饰.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!取消挂接
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!预生成fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!设置LightMap贴图文件名及Info等,MeshID为此模型的网格索引
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh对应的lightmap size，与geometry中mesh的数量一致
		uvTexturePathName: 用于预览uv拆分的结果，如传入"d:\\lightmapUVTest%d.bmp",则
		保存出d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp等文件。
		不用时传NULL
		bReadOnlyModified: 用于返回是否修改了lmm文件的只读属性，可以为NULL
		返回值：成功返回true，失败返回false。如果失败，或者用更大的lightmapsize，或者让美术手动展uv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///后门函数
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;


		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//骨骼渲染矩阵。和逻辑矩阵可能有所不同
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // 这个没有写完
		virtual const char*					GetActionName(unsigned int n){return "testname";} // 这个没有写完
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		得到某一个通道的动画, id合法值为[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // 这个没有写完，需要继续填写
		/*
		清除所有通道的数据
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		设置哪些动作通道为独占通道, buff=NULL会把所有通道设为非独占通道
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 为编辑器添加接口
		//绘制骨骼
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//关联两个物体的动作，使当前物体的动作根骨骼基于骑乘物体某个骨骼,iBoneId是绑定的骨骼ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//在更新骨骼模型的时候,是否对各个骨头进行计算, 默认值为true
		//如果该选项为false, 在ISkeletonModel的copy函数内也不需要拷贝骨头数据.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//设置坐骑矩阵,太特殊了,用于修正马对布料更新的影响
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//获得人物物理成员的名字,传入指定的物理实体类型
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//设置物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理参数,传入物理参数名,目前只支持布料
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!从参数库中删除掉一个参数,目前只支持布料
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//新建一个物理实体
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//删除一个物理实体
		virtual void   DelPhxEntity(const char* pName) {}
		//保存物理数据模板,类型由EPhxDataType指定
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//保存物理数据到bp3中,当前无用
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//加载.传入clearCur表明是否需要清楚掉当前已经读取的参数
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//获取当前所有布料参数组
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//融合物理结果
		virtual void   BlendPhxResult() {}
		//绘制物理
		virtual void   RenderPhxInfo() {}

		//填加一个附加在骨骼上的四元数，在动作计算之后，将旋转附加到骨骼上
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//清空所有的四元数
		virtual void	ClearAdditionalQuat() {}
		//获取布料光芒文件数据
		//返回：光芒数据值针
		//参数：psize，输出型参数，返回光芒数据大小。
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//缩放骨骼,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//动态设置该人物的物理
		virtual void                SetThisActorPhy(bool b){}
		//获得该人物的物理状态，注意，在人物一创建 就需要显示的设置一次SetThisActorPhy，这样每次的GetThisActorPhy才能生效
		virtual bool				GetThisActorPhy(){return false;}

		// 给某个mesh设置排序补偿值
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// 使某个mesh高亮显示
		// bHightLight true : 设置高光效果，false : 恢复
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//更新蒙皮到预定义姿态
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}

		virtual void					Release()
		{
			EnginePool::m_actor_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual bool				SetBodyPart(const char* sBodyName){return true;}
		virtual unsigned int		SetBodyPartAsyn(const char* sBodyName){return 0;}
		virtual void				SetSkinColor(const float* color){}
		virtual unsigned int         GetCloth(ICloth** buff, unsigned int buffCount) { return 0;}
		//add by 2007-12
		//设置ACTOR位置，但是Z永远被设置为0
		virtual void				SetActorLocationMatrix(const H3DMat4& mat){};
		//当BODYPART都调入后，拼接BODYPART之间的顶点和法线
		virtual void				PostProcessCreating(){};
		virtual unsigned int		PostProcessCreatingAsyn(){return 0;};
		//改变IACTOR内容，变成另外一个actor
		virtual int					ChangeToOtherActor(IActor* actor){return 0;};
		//在这里，out实际上就是&this ,用来表明clone已经完成
		virtual unsigned int		ChangeToOtherActorAsyn(IActor* actor,IActor** out){return 0;};

		//动态设置透明排序
		virtual	void				SetThisActorClothSort(bool b){};
		virtual bool				GetThisActorClothSort(){return false;};

		///by sssa2000 2008.04.25
		// 如果头发穿帮了，那么调用该函数即可
		virtual void				ResetActorHair(){};

		//调试函数
		virtual void				DebugFunction(int i){};
		//add by puzzy, 2008-10-10
		//H3DSet geometry lod and texture lod
		virtual void				setGeometryLod(float lod){};
		virtual unsigned int		setFragmentLodAsyn(float n){return 0;};
		virtual void				setFragmentLod(float ){};
		virtual float				getGeometryLod(){return 0;};
		virtual float				getFragmentLod(){return 0;};

		virtual int GetSkeletonModels(IModel** pSkeletonModelVector,int vectorSize) {return 0;}
		virtual int GetAdornmentSkeletonModels(H3DI::IModel** pSkeletonModelVector, int vectorSize) {return 0;}

		//设置人物是否使用包围体. 返回值1表示成功，小于0不成功
		virtual int SetUseCustomBoundBox (bool b){return 0;}

		//通过bodpyart的名字获取该bodypart 当前在引擎中所有renderable的所有材质。将材质填充到pResArray
		//如果引擎要返回的材质数量大于调用者分配的数组的长度，返回值为-1
		//返回值：返回实际填充的材质个数
		//参数bpName：bodypart的名字，例如104001003，如果是挂件 也类似
		//参数pResArray:由调用者分配的INewShader的数组，
		//参数len:数组的长度
		virtual int GetAllShaderByBodyPartName(const char* bpName,INewShader** pResArray,int arraylen){ return -1;}

		//根据bodypart名字获取INewShader
		// 设置挂件
		virtual bool				SetAdornment(EActorAdornmentPosition adornmentposition, const char* adornmentname) {return true;}
	};

	class CAvatarSkeletonModel:public IAvatarSkeletonModel
	{
	public:

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;
//MOVE OBJECT
		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

//MODEL
		int m_flag;
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//运行控制

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//可视范围
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//获得一个模型号的顶点和索引数据，
		//pVertBuf，pIndexBuff，顶点和索引的指针，用于传出数值
		//pVertNum,pIndexNum顶点和索引的个数，当前pVertBuf，pIndexBuff都传入空指针是，返回顶点和索引的个数。一般情况下传入顶点和索引的个数。
		//返回值：当传入的顶点和索引的数量大于等于实际的模型所需的数量时返回真,否则返回假
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///重新载入这个模型的材质,返回1表示成功，0表示失败。
		virtual int ReloadMaterialInstance(){return 0;}
		///重新载入fx
		virtual int ReloadFx(){return 0;}

		///设置模型的模糊因子。0.0-1.0的浮点数。为0时表示模型完全清晰，为1是表示完全参与运动模糊。0-1之间的数表示参与运动模糊的程度。
		virtual void SetMotionBlurFactor(float fFactor) {}
		///设置遮挡查询的状态
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///获取是否使用遮挡查询的状态
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///获得查询结果，0表示被完全遮挡了
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//材质相关

		//高亮参数是2个颜色c0,c1，最终的颜色是c0*物体颜色+c1
		//返回值1表示成功，0表示失败。失败的原因是物体的材质不支持该操作
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///得到新材质，参数是网格的id，从0开始
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///得到新材质，参数是网格的名字
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///设置新材质 将影响到整个模型的所有mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给整个模型，将影响到整个模型的所有mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///设置新材质 将一个材质设置给某个mesh，返回0表示失败，可能是没有该mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///通过一个材质实例的文件名和材质实例的名字 将一个材质设置给某个mesh
		///返回值是从文件中得到的INewShader,0表示失败
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///从内存设置一个材质模板给模型，模型的所有网格都受到影响。返回1表示成功
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到改名字的网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///从内存设置一个材质模板给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//以下接口不要使用

		///设置物体是否具有反射效果
		///参数b:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///获取物体是否具有反射效果
		///返回:是否开启反射
		///参数plane：是一个4个元素的数组，保存的是平面的方程
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///设置物体是否参与反射渲染
		virtual void SetCanBeReflection(bool b){}

		///获取物体是否参与反射渲染
		virtual bool GetCanBeReflection(){ return false;}

		///设置反射远平面
		///参数：远平面距离
		virtual void SetReflectionFarClipPlane(float dis){}

		///设置物体是否是折射物体
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///设置折射率
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//编辑器生成光芒时调用的接口
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//获取光芒文件数据
		//	返回：光芒数据值针
		//	参数：psize，输出型参数，返回光芒数据大小。
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//功能：计算一条射线与模型上的包围盒相碰撞的信息
		//参数
		//rayOrigin：传入三个浮点数组成的数组，表示一条射线的起始点
		//rayDir：传入三个浮点数组成的数组，表示一条射线的方向
		//ModelBVTestInfo ：一个数组首地址，用于结果的返回。所有碰撞到的包围盒按先后顺序将ID和碰撞点坐标返回给客户端。数组的大小不能小于一个模型的包围盒个数，包围盒个数由引擎再提供一个接中得到
		//nResultBufferCount：数组的元素个数，防止引擎填充数据越界
		//返回值：返回的结果的个数,如果为，说明没有碰撞
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//获取包围体的个数
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//功能：射线求交
		//	//参数
		//	RayOrg：传入的射线起点
		//	RayDir：传入射线方向
		//	fOutDistance：引擎返回的结果，这个数值是碰撞点到射线起点的距离。
		//返回值：射线与模型是否碰撞
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!挂饰相关接口
		//根据射线得到挂接数据集
		//第二个参数表示是否需要转换射线空间
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!挂接挂饰.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!取消挂接
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!预生成fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!设置LightMap贴图文件名及Info等,MeshID为此模型的网格索引
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh对应的lightmap size，与geometry中mesh的数量一致
		uvTexturePathName: 用于预览uv拆分的结果，如传入"d:\\lightmapUVTest%d.bmp",则
		保存出d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp等文件。
		不用时传NULL
		bReadOnlyModified: 用于返回是否修改了lmm文件的只读属性，可以为NULL
		返回值：成功返回true，失败返回false。如果失败，或者用更大的lightmapsize，或者让美术手动展uv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///后门函数
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;
//skeletonmodel
		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//骨骼渲染矩阵。和逻辑矩阵可能有所不同
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // 这个没有写完
		virtual const char*					GetActionName(unsigned int n){return "testname";} // 这个没有写完
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		得到某一个通道的动画, id合法值为[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // 这个没有写完，需要继续填写
		/*
		清除所有通道的数据
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		设置哪些动作通道为独占通道, buff=NULL会把所有通道设为非独占通道
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 为编辑器添加接口
		//绘制骨骼
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//关联两个物体的动作，使当前物体的动作根骨骼基于骑乘物体某个骨骼,iBoneId是绑定的骨骼ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//在更新骨骼模型的时候,是否对各个骨头进行计算, 默认值为true
		//如果该选项为false, 在ISkeletonModel的copy函数内也不需要拷贝骨头数据.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//设置坐骑矩阵,太特殊了,用于修正马对布料更新的影响
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//获得人物物理成员的名字,传入指定的物理实体类型
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//设置物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理实体的属性,当是布料参数时,传入的是布料网格名
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//获取物理参数,传入物理参数名,目前只支持布料
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!从参数库中删除掉一个参数,目前只支持布料
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//新建一个物理实体
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//删除一个物理实体
		virtual void   DelPhxEntity(const char* pName) {}
		//保存物理数据模板,类型由EPhxDataType指定
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//保存物理数据到bp3中,当前无用
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//加载.传入clearCur表明是否需要清楚掉当前已经读取的参数
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//获取当前所有布料参数组
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//融合物理结果
		virtual void   BlendPhxResult() {}
		//绘制物理
		virtual void   RenderPhxInfo() {}

		//填加一个附加在骨骼上的四元数，在动作计算之后，将旋转附加到骨骼上
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//清空所有的四元数
		virtual void	ClearAdditionalQuat() {}
		//获取布料光芒文件数据
		//返回：光芒数据值针
		//参数：psize，输出型参数，返回光芒数据大小。
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//缩放骨骼,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//动态设置该人物的物理
		virtual void                SetThisActorPhy(bool b){}
		//获得该人物的物理状态，注意，在人物一创建 就需要显示的设置一次SetThisActorPhy，这样每次的GetThisActorPhy才能生效
		virtual bool				GetThisActorPhy(){return false;}

		// 给某个mesh设置排序补偿值
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// 使某个mesh高亮显示
		// bHightLight true : 设置高光效果，false : 恢复
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//更新蒙皮到预定义姿态
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}



		virtual void					Release()
		{
			EnginePool::m_avaskeleton_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

//avatarskeletonmodel
		//!设置bodypart，参数是bpt文件的路径
		virtual bool SetBodyPart(const char* bpt_path){return true;};
		virtual unsigned int SetBodyPartAsyn(const char* bpt_path){return 0;};

		//!设置挂件，参数0是位置的索引，参数1是spe文件的路径
		virtual bool SetAdornment(EPetAdornmentPosition adornmentposition, const char* adornmentname){return true;};
		virtual unsigned int SetAdornmentAsyn(int pos_idx,const char* spe_path){return 0;};

		//!lod相关
		virtual void				setGeometryLod(float lod){}
		virtual unsigned int		setFragmentLodAsyn(float n){return 0;};
		virtual void				setFragmentLod(float ){}

		virtual float				getGeometryLod(){return 0;};
		virtual float				getFragmentLod(){return 0;};

		virtual int GetSkeletonModels(IModel** pSkeletonModelVector,int vectorSize) { return 0;};
		virtual int GetAdornmentSkeletonModels(H3DI::IModel** pSkeletonModelVector, int vectorSize) { return 0;};

		//设置人物是否使用包围体. 返回值1表示成功，小于0不成功
		virtual int SetUseCustomBoundBox (bool b){return 0;}; 

		//调试函数
		virtual void				DebugFunction(int i){ };
	};



	class CPrePassLight : public IPrePassLight
	{
	public:
		CPrePassLight():m_light_type(LIGHT_DIR)
		{
		}

		LIGHT_TYPE m_light_type;

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		virtual void					Release()
		{
			EnginePool::m_light_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}


		virtual void SetLightAffectParam(LightAffectParam t){};
		virtual LightAffectParam GetLightAffectParam(){return LightAffectParam();};

		virtual LIGHT_TYPE				GetLightType()const{return m_light_type;};

		virtual void                    GetColor(float* vec4) const {};
		virtual void					SetColor(const float* col) {};

		//	光强,方向光光强分为两个，分别影响固有色和高光，其他类型光、延迟渲染只使用第一个光强。
		//	获取光强index=1时取高光光强SpecIntensity,index=0时去原始光强intensity
		virtual void					SetIntensity(float intensity, float SpecIntensity=-1.0f) {};
		virtual float					GetIntensity(int index=0) const {return 1;};


		// 聚光灯的内外角度，光强从内角开始衰减，到外角无光照。单位为角度
		virtual void					GetAngle(float* inner, float* outter) const{};
		virtual void					SetAngle(float inner, float outter) {};

		// 设置这个灯是否产生阴影以及阴影的颜色
		virtual bool					GetShadowEnable() const {return true;};
		virtual void					SetShadowEnable(bool bEnable) {};

		virtual void					GetShadowColor(float* vec4) const{};
		virtual void					SetShadowColor(const float* col) {};

		// 衰减范围，从near开始衰减，到far结束
		virtual void					GetRange(float* nearRange, float* farRange) const {};;
		virtual void					SetRange(float nearRange, float farRange) {};;

		// 优先级
		virtual int						GetPriority() const {return 1;};
		virtual void					SetPriority(int nPriority) {};

		// 是否使用投影贴图
		virtual bool					GetProjTextureEnable() const {return false;};
		virtual void					SetProjTextureEnable(bool bEnable) {};
		// 投影贴图的文件名
		virtual void					SetProjTextureName(const char* strFileName){};

		// 是否使用光斑贴图
		virtual bool					GetFakeLightSpot() const {return false;};
		virtual void					EnableFakeLightSpot(bool bEnable) {};
		// 光斑贴图的文件名
		virtual void					SetFakeLightSpotTexture(const char* strFileName) {};

		//创建一个光柱
		virtual void CreateLightBeam(EH3DBEAMTYPE beamType = E_H3D_BEAM_VOL) {};
		//设置光柱类型
		virtual void SetLightBeamType(EH3DBEAMTYPE beamType) {};
		//释放光柱.会释放掉当前已经创建的所有光柱类型
		virtual void ReleaseLightBeam() {};
		//创建一个光斑.由于一个灯光会有多个光斑,所以这里返回光斑的索引
		virtual int CreateGlare(const tGlareProperty& gPro) {return 0;};
		//释放光斑
		virtual void ReleaseGlare(int glareIndex) {};
		//设置光柱本地矩阵
		virtual void SetBeamLocalMat(const H3DMat4& mat) {};
		//设置光斑的属性
		virtual void SetGlareProperty(int glareIndex,const tGlareProperty& gPro) {};
		//获取光斑的属性
		virtual void GetGlareProperty(int glareIndex,tGlareProperty& gPro) {};
		//设置光柱的可见性
		virtual void SetBeamVisable(bool b) {};
		//启用/停用光源
		virtual void SetLightEnable(bool b) {}
		//
		virtual bool GetLightEnable() {return true;}
		//设置光柱的明暗
		virtual void SetBeamBrightness(float t) {}
		//设置光柱的内径（0.1~1）
		virtual void SetBeamInnerRadius(float t) {}
		//设置后处理光柱的模糊度(0~1)
		virtual void SetBeamBlurCoeff(float f) {}

		// 光柱衰减范围，从near开始衰减，到far结束
		virtual void GetBeamRange(float* nearRange, float* farRange) const {}
		virtual void SetBeamRange(float nearRange, float farRange) {}
		// 光柱内外角度，单位为角度
		virtual void GetBeamAngle(float* inner, float* outter) const {}
		virtual void SetBeamAngle(float inner, float outter) {}
		//===============================LightShaft===================================
		//add by liren 2010-11-2 
		//!开启或关闭体积光
		virtual void SetLightShaftEnable(bool bEnable) {}
		//!设置体积光颜色
		virtual void SetLightShaftColor(const float *pcolor){}
		//!设置体积光的深度 
		virtual void SetLightShaftDepth(float fDepth) {}
		//!体积光是否开启
		virtual bool GetLightShaftEnable() const {return false;}
		//!获取体积光颜色
		virtual void GetLightShaftColor(float *pColor) const {}
		//!获取体积光的深度 
		virtual float GetLightShaftDepth() const {return 1;}

		//!设置体积光阈值
		virtual void SetLightShaftThreshold(float fThreshold) {}
		//!设置体积光阈值
		virtual float GetLightShaftThreshold()const {return 1;}
		//!设置体积光强度
		virtual float GetLightShaftIntensity() const {return 1;}
		//!获取体积光颜色
		virtual void SetLightShaftIntensity(float fIntensity) {}
		//===============================!LightShaft===================================

		//!设置灯光的GUID
		virtual void SetGUID(const char* szGUID) {}
	};

	class CCamera : public ICamera
	{
	public:
		H3DVec3 m_c_v;

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//运动控制
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
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
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//层次关系控制，上升到IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///重新载入所有,返回1表示成功，0表示失败。
		virtual int						ReloadAll(){return 1;}

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;}

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//用AABB测试是否和某个射线相交。point和dir都是x、y、z三个分量
		//返回值为1表示相交
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//设置颜色系数
		//f是颜色系数的数组，arraysize是颜色系数数组长度
		//返回值=1表示成功，返回值小于0表示不成功，不成功的原因是材质中没有定义颜色系数
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// 设置固有色颜色系数,要求传入两个float[4]，影响方法： (Mat_d*C0.rgb)+C1.rgb，fC0的Alpha用于饱和度计算，fC1的Alpha用于亮度计算
		/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
		///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
		/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//为模型设置渲染参数。参数是EEntityFlag的按位运算的结果
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///设置材质lod,nLod=0即为最高细节，nLod=2为最简单细节
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		virtual void					Release()
		{
			EnginePool::m_camera_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		//!得到向上的方向
		virtual const H3DVec3& GetUp(void) {return m_c_v;};
		//!得到向右的方向
		virtual const H3DVec3& GetRight(void) {return m_c_v;};
		//!从当前位置向position看
		virtual void LookAt(const H3DVec3& position) {}
		//!从eyePos向atPos看
		virtual void LookAt(const H3DVec3& eyePos, const H3DVec3& atPos, const H3DVec3& upVec) {}
		//!绕着前方向转,给角度
		virtual void Roll(float degree) {}
		//!绕着右方向转,给角度
		virtual void Yaw(float degree) {}
		//!绕着上方向转,给角度
		virtual void Pitch(float degree) {}
		//!绕着axis转degree角度
		virtual void Rotate(const H3DVec3& axis, float degree) {}
		//!向上移动step距离
		virtual void MoveUp(float step) {}
		//!向右移动step距离
		virtual void MoveRight(float step) {}
		//!向前移动step距离
		virtual void MoveFront(float step) {};
		//!得到view martix
		virtual const H3DMat4& GetViewMatrix(void)const {return m_d_m;};
		//!得到投影matrix
		virtual const H3DMat4& GetProjectionMatrix(void)const {return m_d_m;};

		virtual ~CCamera(){}


	};


	// CPrePassLight t;
	//CActor tt;
	//CCamera ttt;

}
#endif