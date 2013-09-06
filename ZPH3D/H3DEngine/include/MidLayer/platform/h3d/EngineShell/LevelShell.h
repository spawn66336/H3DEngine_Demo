#ifndef LEVELSHELL
#define LEVELSHELL

#include "instance_pool.h"

namespace H3DI
{
	class  CLevel: public ILevel
	{
	public:
		//OBJECT
		virtual void					Release()
		{
			EnginePool::m_level_pool->releaseObj(this);
		};

		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};
		//MOVE OBJECT
		virtual MODEL_TYPE				Type(){return MODEL_STATIC;};
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

		/*参数：
		0：等同于ReloadAll
		1：Reload Geometry
		2：Reload 材质（包括贴图）
		3：Reload 贴图

		返回值-1为失败，大于0的数表示reload了几张贴图
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;};
		///获得本物体使用的显存
		virtual unsigned int GetVideoMemUse(){return 0;};

		///获得性能数据
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

		///从内存设置一个材质实例给模型的一个网格，返回1表示成功，0表示找不到该网格
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 1;};

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

		//SCENE
		//把一个物体加入场景中
		virtual void					AttachModel(IMoveObject* pObj,int nLayer){};

		//把一个物体从场景删除
		virtual void					DetachModel(IMoveObject* pObj){};

		//当物体移动时，必须使物体在场景中移动！ 只有CLIENT知道MODEL是否移动了
		//当物体没有移动时，不要调用此函数。
		virtual void					MoveModel(IMoveObject* pObj){};	

		//!!!!!! ISceneSearch must be Released by CLIENT !!!!!!
		//use aabb to search
		//使用AABB搜索
		virtual ISceneSearch*			FindObjects(const float* pAABB,const int* pLayers,int numLayer){return 0;};
		//使用AABB在X，Y平面上搜索，不考虑Z范围
		virtual ISceneSearch*			FindObjects2D(const float* pAABB,const int* pLayers,int numLayer){return 0;};
		//use IRender's camera to search
		virtual ISceneSearch*			FindObjects(IRender* pRenderer,const int* pLayers,int numLayer){return 0;};
		//以锥体来搜索，在各种RENDER TO IMAGE的特效中，和SPOT LIGHTING中需要
		//virtual ISceneSearch			FindObjects(tClipPlanes[5],const int* pLayers,int numLayer){}


		//根据MATRIX得到CAMERA信息（未实现03-12-3)
		virtual void					SetCurrentCamera(const float* pViewMatrix,const float* pProjectMatrix){};
		//根据位置和角度得到CAMERA信息（未实现03-12-3)
		virtual void					SetCurrentCamera(const float* vecEyePos,float fov,float ratio,float near, float far){};
		//根据IRENDER中当前的CAMERA来设置场景CULLING CAMERA，推荐使用
		virtual void					SetCurrentCamera(IRender* pRenderer){};
		//LEVEL
		//!得到AABB
		virtual void GetWorldAABB(float* pArray6)const {  };
		//!设置环境光颜色
		virtual void SetAmbientColor(float r, float g, float b) {  };
		//!读geometry
		virtual bool Load(const char* fileName) { return true; };
		//!用相机culling
		virtual void CullByCamera(ICamera*) {  };

		//by sssa2000 081203
		//获得调试信息
		virtual unsigned int GetOctreeAABBTestNum(){return 0;};
		virtual unsigned int GetSubMeshOctreeSectionTestNum(){return 0;};

		//by sssa2000 090216
		//Condition1和Condition2能够组合起来绘制满足2个条件的结点包围盒
		virtual void SetDebugCondition1(LEVEL_DEBUG_CONDITION ldc){};
		virtual void SetDebugCondition2(LEVEL_DEBUG_CONDITION ldc){};
		virtual void SetDebugCalc(LEVEL_DEBUG_SET_CALC ldc){};
		virtual void SetDrawOctreeLevel(int i){}; //设置绘制octree的包围盒的层级。为-1时绘制所有层级
		//by sssa2000 090209
		virtual void EnableOccluder(bool b){};
		virtual bool IsOccluderEnable(){return 0;};
		virtual int GetOccluderNumber(){return 0;};
		virtual void DrawOccluder(int i){};
		virtual void DrawOccluderFrustum(int i,H3DVec3& viewPoint){};
		//根据场影中细节物体重新分割八叉树
		virtual void RestructOctree(){};
		//在一个场景中添加后处理
		virtual void			AddPostProcess(const char* filename){};
		//在一个删除后处理
		virtual void			DelPostProcess(const char* filename){};
		//设置一个场景的后处理参数
		virtual void			SetPostProcessParam(const char *name,const void *pvalue,int sizeOfValue){};
		//设置一个场景的视口
		virtual	void			SetViewPort(const unsigned short *vp) {  };
		//得到一个场景的视口,传入一个四个元素的数组
		virtual	void			GetViewPort(unsigned short *vp) {  };
		//设置视矩阵
		virtual void			SetViewMatrix(const H3DMat4 &mat) {  };
		//设置透视矩阵
		virtual void			SetPerspectiveMatrix(const H3DMat4 &mat) {  };

		//视矩阵
		virtual void					GetViewMatrix(H3DMat4 &mat) {  };
		//透视矩阵
		virtual void					GetPerspectiveMatrix(H3DMat4 &mat) {  };
		virtual void					SetEyePosition(const H3DVec3& pos) {  };
		virtual void					GetEyePosition(H3DVec3& pos) {  };
		virtual void					LookAt(const H3DVec3& eyepos, const H3DVec3& at, const H3DVec3& up) {  };

		virtual void					RotateDirection(float yaw_offset,float pitch_offset,float roll_offset){};
		virtual void					SetCameraRotation(float yaw,float pitch,float roll){};
		virtual void					GetRotateDir(float& yaw,float& pitch,float& roll){};

		virtual void					GetEyeDirection(H3DVec3& vec) {  };
		virtual void					SetFrustum(float fov,float ratio,float fNear,float fFar,ECameraType camType = ECT_Render){};
		virtual void					GetFrustum(float& fov,float& ratio,float& fNear,float& fFar,ECameraType camType = ECT_Render){};
		virtual void					UpdateCamera(){};

		/// 设置场景环境光颜色
		virtual void					SetAmbientColor(const float *col){};
		/// 设置角色环境光颜色			
		virtual void					SetActorAmbientColor(const float *col){};
		//设置次表面散射颜色
		virtual void					SetTransmissionColor(const float* col) {  };
		/// 设置场景材质LOD
		//virtual void					SetMaterialLod(int nLod){};
		/// 得到材质LOD			
		//virtual int						GetMaterialLod(){return 0;};

		virtual void					SetShadowFadeoutEnd(float fParam) {  };

		// 影响lightmap颜色 lm' = lm * a + b
		// a, b均为float3
		virtual void				SetLightmapColorFactor(const float* a, const float* b) {  };


		virtual ~CLevel(){}
	};
}
#endif