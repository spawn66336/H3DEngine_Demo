/********************************************************************
	created:	2009/08/06
	created:	6:8:2009   10:41
	filename: 	f:\NewEngineLod_MT\EngineLod\SDK\Include\Engine\H3DSTLHeader.h
	file path:	f:\NewEngineLod_MT\EngineLod\SDK\Include\Engine
	file base:	H3DSTLHeader
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <hash_map>
#include <hash_set>
#include <iterator>
#include <utility>
#include <sstream>
#include <iomanip>
// #include "D3DInclude.h"
// #include "MyAllocator.h"
#ifdef USE_NEDMALLOC
#include "nedmelloc/nedmalloc.h"
using namespace nedalloc;
#endif

#ifdef USE_STLPORT
//#include "stlport/stl/config/"
#endif

namespace H3D
{

//#define  USE_H3D_STL


#ifdef ID_REDIRECT_NEWDELETE
	typedef std::basic_string<char, std::char_traits<char>, CMyAllocator<char> > H3DString;
	typedef std::basic_stringstream<char, std::char_traits<char>,CMyAllocator<char> > H3DStringStream;
	typedef std::basic_ostringstream<char, std::char_traits<char>,CMyAllocator<char> > H3DOStringStream;
	typedef std::basic_istringstream<char, std::char_traits<char>,CMyAllocator<char> > H3DIStringStream;

	template <class T>
	class H3DVector:public std::vector<T,CMyAllocator<T> >
	{
	public:
		H3DVector():std::vector<T,CMyAllocator<T> >(){}
		H3DVector(size_type _Count, const T& _Val):std::vector<T,CMyAllocator<T> >(_Count,_Val){}
		explicit H3DVector(size_type _Count):std::vector<T,CMyAllocator<T> >(_Count){}
	};

	template <class T>
	class H3DList:public std::list<T,CMyAllocator<T> >
	{};

	template <class T1,class T2,class _pr = std::less<T1> >
	class H3DMap:public std::map<T1,T2,_pr,CMyAllocator<std::pair<const T1, T2> > >
	{};


	template<class T1,class T2,class _pr = std::less<T1> >
	class H3DMulimap:public std::multimap<T1,T2,_pr,CMyAllocator<std::pair<const T1,T2>>>
	{};


	template <class T>
	class H3DDeque:public std::deque<T,CMyAllocator<T>>
	{};

	template<class T1,class T2,class _Tr = stdext::hash_compare<T1, std::less<T1> > >
	class H3DHash_map:public stdext::hash_map<T1,T2,_Tr,CMyAllocator<std::pair<const T1,T2> > >
	{};

	template<class T,class _Tr = stdext::hash_compare<T, std::less<T> > >
	class H3DHash_set:public stdext::hash_set<T,_Tr,CMyAllocator<T> >
	{};


	template<class T,class _Pr = std::less<T> >
	class H3DSet:public std::set<T,_Pr,CMyAllocator<T> >
	{};


	template<class T,class _Pr = std::less<T> >
	class H3DMuliset:public std::multiset<T,_Pr,CMyAllocator<T> >
	{};


#else
#ifdef USE_NEDMALLOC
	typedef std::basic_string<char, std::char_traits<char>, nedallocator<char> > H3DString;
	typedef std::basic_stringstream<char, std::char_traits<char>,nedallocator<char> > H3DStringStream;
	typedef std::basic_ostringstream<char, std::char_traits<char>,nedallocator<char> > H3DOStringStream;
	typedef std::basic_istringstream<char, std::char_traits<char>,nedallocator<char> > H3DIStringStream;

	template <class T>
	class H3DVector:public std::vector<T,nedallocator<T> >
	{
	public:
		H3DVector():std::vector<T,nedallocator<T> >(){}
		H3DVector(size_type _Count, const T& _Val):std::vector<T,nedallocator<T> >(_Count,_Val){}
		template<class _H3DIter>
		H3DVector(_H3DIter _First, _H3DIter _Last):std::vector<T,nedallocator<T> >(_First, _Last){}
		explicit H3DVector(size_type _Count):std::vector<T,nedallocator<T> >(_Count){}
	};

	template <class T>
	class H3DList:public std::list<T,nedallocator<T> >
	{};

	template <class T1,class T2,class _pr = std::less<T1> >
	class H3DMap:public std::map<T1,T2,_pr,nedallocator<std::pair<const T1, T2> > >
	{};


	template<class T1,class T2,class _pr = std::less<T1> >
	class H3DMulimap:public std::multimap<T1,T2,_pr,nedallocator<std::pair<const T1,T2>>>
	{};


	template <class T>
	class H3DDeque:public std::deque<T,nedallocator<T>>
	{};

	//template <class _Key, class _Tp>
	//class H3DHash_map:public _STLP_STD_NAME::hash_map
	//	<
	//	_Key,
	//	_Tp,
	//	_STLP_STD_NAME::hash<_Key>,
	//	_STLP_STD_NAME::equal_to<_Key>,
	//	nedallocator<std::pair<const _Key,_Tp> >  
	//	>
	//{};

	//template <class _Tp>
	//class H3DHash_set:public _STLP_STD_NAME::hash_set
	//	<
	//	_Tp,
	//	_STLP_STD_NAME::hash<_Tp>,
	//	_STLP_STD_NAME::equal_to<_Tp>,
	//	nedallocator<_Tp >  
	//	>
	//{
	//public:
	//	H3DHash_set():_STLP_STD_NAME::hash_set
	//		<
	//		_Tp,
	//		_STLP_STD_NAME::hash<_Tp>,
	//		_STLP_STD_NAME::equal_to<_Tp>,
	//		nedallocator<_Tp >  
	//		>
	//		(){}
	//	template<class _H3DIter>
	//	H3DHash_set(_H3DIter _First, _H3DIter _Last):_STLP_STD_NAME::hash_set
	//		<
	//		_Tp,
	//		_STLP_STD_NAME::hash<_Tp>,
	//		_STLP_STD_NAME::equal_to<_Tp>,
	//		nedallocator<_Tp >  
	//		>
	//		(_First, _Last){}
	//};



	template<class T,class _Pr = std::less<T> >
	class H3DSet:public std::set<T,_Pr,nedallocator<T> >
	{};


	template<class T,class _Pr = std::less<T> >
	class H3DMuliset:public std::multiset<T,_Pr,nedallocator<T> >
	{};
#else
	typedef std::string H3DString;
	typedef std::stringstream H3DStringStream;
	typedef std::ostringstream H3DOStringStream;
	typedef std::istringstream H3DIStringStream;

	template <class T>
	class H3DVector:public std::vector<T>
	{
	public:
		H3DVector():std::vector<T>(){}
		H3DVector(size_type _Count, const T& _Val):std::vector<T>(_Count,_Val){}
		template<class _H3DIter>
		H3DVector(_H3DIter _First, _H3DIter _Last):std::vector<T>(_First, _Last){}
		explicit H3DVector(size_type _Count):std::vector<T>(_Count){}
	};

	template <class T>
	class H3DList:public std::list<T>
	{};

	template <class T1,class T2,class _pr = std::less<T1> >
	class H3DMap:public std::map<T1,T2,_pr>
	{};




	template<class T1,class T2>
	class H3DMulimap:public std::multimap<T1,T2,std::less<T1> >
	{};


	template <class T>
	class H3DDeque:public std::deque<T>
	{};

	template<class T1,class T2>
	class H3DHash_map:public std::map<T1,T2,std::less<T1> >
	{};

	template<class T>
	class H3DHash_set:public std::set<T,std::less<T> >
	{
	public:
		H3DHash_set():std::set<T>(){}
		template<class _H3DIter>
		H3DHash_set(_H3DIter _First, _H3DIter _Last):std::set<T>(_First, _Last){}
	};


	template<class T>
	class H3DSet:public std::set<T,std::less<T> >
	{};


	template<class T>
	class H3DMuliset:public std::multiset<T,std::less<T> >
	{};

#endif //USE_NEDMALLOC
#endif // USE_H3D_STL




}