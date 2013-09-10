#ifndef ZP_H3DDRESSSELECTOR
#define ZP_H3DDRESSSELECTOR

#include "ZPDependency.h"
#include "engine_interface.h"

class TiXmlElement;

namespace ZPH3D
{ 

	const int ACTOR_ITEM_NUM = H3DI::BODYPART_NUM+H3DI::ACTOR_ADORNMENT_NUM;

	//宠物的bodypart枚举中没有套装，所以要+1
	const int PET_ITEM_NUM = H3DI::PET_BODYPART_NUM+1+H3DI::PET_ADORNMENT_NUM;


	class H3DDressInfo
	{
	public:
		H3DDressInfo():m_iType(0){}
		H3DDressInfo( const H3DDressInfo& info );
		~H3DDressInfo(){}

		const String& GetID( void ) const { return m_id; }
		void SetID( const String& id ){ m_id = id; }

		int GetType( void ) const { return m_iType; }
		void SetType( const int type ){ m_iType = type; }

		const String& GetPath( void ) const { return m_strResPath; }
		void SetPath( const String& path ){ m_strResPath = path; }

		H3DDressInfo& operator=( const H3DDressInfo& rhs );

	protected:
		String m_id;					//服装ID
		int m_iType;					//服装类型
		String m_strResPath;    //服装资源路径
	};


	class H3DDressSelector
	{
	public: 
		typedef std::vector<H3DDressInfo> dressList_t;
		typedef std::vector<dressList_t> dressTypeList_t;
	public:
		H3DDressSelector(void);
		virtual ~H3DDressSelector(void); 

		void LoadDressesFromXmlFile( const String& filename );

	protected:
		
		void	_ParseActorDressList( TiXmlElement* pXmlElem );
		void	_ParseActorDress( const bool male , TiXmlElement* pXmlElem );
		dressTypeList_t& _GetActorDressTypeList( const bool male );

	protected:
		dressTypeList_t m_maleDressTypeList;	
		dressTypeList_t m_femaleDressTypeList;
	};

}//namespace ZPH3D

#endif //ZP_H3DDRESSSELECTOR