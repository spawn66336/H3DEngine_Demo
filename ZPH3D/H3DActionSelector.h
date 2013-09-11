#ifndef ZP_H3DACTIONSELECTOR
#define ZP_H3DACTIONSELECTOR

#include "ZPDependency.h"
#include "engine_interface.h"

class TiXmlElement;

namespace ZPH3D
{

	class H3DActionInfo
	{
	public:
		H3DActionInfo( void );
		H3DActionInfo( const H3DActionInfo& info );
		~H3DActionInfo();

		String GetID(void) const { return m_strID; }
		void SetID( const String& id ){ m_strID = id; }

		String GetFileName( void ) const { return m_strFileName; }
		void SetFileName( const String& filename ){ m_strFileName = filename; }

		H3DActionInfo& operator=( const H3DActionInfo& rhs );

	protected:
		String m_strID;
		String m_strFileName;
	};
	
	class H3DActionSelector
	{
	public:
		typedef std::vector<H3DActionInfo> actionList_t;
	public:
		H3DActionSelector(void);
		virtual ~H3DActionSelector(void);

		void LoadActionsFromXMLFile( const String& filename );

		String GetCurrAction( const bool male );

		void NextAction( void );

	protected:

		void _ParseActionNode( TiXmlElement* pElem );

		actionList_t& _GetActionList( const bool male );

		bool _IsActionResourceExist( const String& filename );

	protected:
		int			  m_currMaleActionIndx;
		int			  m_currFemaleActionIndx;
		actionList_t m_maleActions;		//男性舞蹈动作
		actionList_t m_femaleActions;  //女性舞蹈动作
	};

}

#endif //ZP_H3DACTIONSELECTOR