#ifndef ZP_H3DDRESSSELECTOR
#define ZP_H3DDRESSSELECTOR
#include "ZPDependency.h"


namespace ZPH3D
{ 

	class H3DDressInfo
	{
	public:
		H3DDressInfo():m_iType(0){}
		H3DDressInfo( const H3DDressInfo& info );
		~H3DDressInfo(){}

		H3DDressInfo& operator=( const H3DDressInfo& rhs );

	protected:
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
		 
	protected:
		dressTypeList_t m_dressTypeList;	
	};

}//namespace ZPH3D

#endif //ZP_H3DDRESSSELECTOR