/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <ctype.h>
#include "tinyxml.h"

#ifdef BIXML_USE_STL
#include <sstream>
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////////
//by sssa2000 2010.01.28
//如果不加上这一段链接不过
//该错误主要是由于静态库在VC6编译而主程序在VC2005编译，大家用的CRT不同
#ifdef __cplusplus
extern "C"
#endif
FILE _iob[3] = {__iob_func()[0], __iob_func()[1], __iob_func()[2]};
//////////////////////////////////////////////////////////////////////////

#include <common_lib/vfs_dll/ifile.h>
//#include "../../../common_lib/vfs_dll/ifile.h"
//#include "mmgrInclude.h"

extern const int g_xml_use_mem_pool;

//属性
BiXmlAttribute*		BiXmlCreateAttribute(const char* name/* = "" */, const char* value/* = "" */)
{
	return BiXmlFactory::CreateAttribute(name, value);
}

void				BiXmlReleaseAttribute(BiXmlAttribute* attr)
{
	attr->Release();
}

//解释
BiXmlComment*		BiXmlCreateComment()
{
	return BiXmlFactory::CreateComment();
}
//声明
BiXmlDeclaration*	BiXmlCreateDeclaration(const char* version/* = "" */, const char* encoding/* = "" */, const char* standalone/* = "" */)
{
	return BiXmlFactory::CreateDeclaration(version, encoding, standalone);
}
//文档
BiXmlDocumentImpl*	BiXmlCreateDocument(const char* name/* = "" */)
{
	return BiXmlFactory::CreateDocument(name);
}
//元素
BiXmlElementImpl*	BiXmlCreateElement(const char* _value)
{
	return BiXmlFactory::CreateElement(_value);
}
//文本
BiXmlTextImpl*			BiXmlCreateText(const char* text)
{
	return BiXmlFactory::CreateText(text);
}
//未知
BiXmlUnknown*		BiXmlCreateUnknown()
{
	return BiXmlFactory::CreateUnknown();
}

void BiXmlReleaseDocument(BiXmlDocumentImpl* doc)
{
	//BiXmlFactory::ReleaseDocument(doc);
	doc->Release();
}

void BiXmlReleaseElement(BiXmlElementImpl* elem)
{
	//BiXmlFactory::ReleaseElement(elem);
	elem->Release();
}

ClassPool<BiXmlAttribute, CBiboLock>	BiXmlFactory::m_AttributePool;
ClassPool<BiXmlComment, CBiboLock>		BiXmlFactory::m_CommentPool;
ClassPool<BiXmlDeclaration, CBiboLock>	BiXmlFactory::m_DeclarationPool;
ClassPool<BiXmlDocumentImpl, CBiboLock>	BiXmlFactory::m_DocumentPool;
ClassPool<BiXmlElementImpl, CBiboLock>	BiXmlFactory::m_ElementPool;
ClassPool<BiXmlTextImpl, CBiboLock>			BiXmlFactory::m_TextPool;
ClassPool<BiXmlUnknown, CBiboLock>		BiXmlFactory::m_UnknowPool;

BiXmlAttribute* BiXmlFactory::CreateAttribute(const char* name /* = "" */, const char* value /* = "" */)
{
	if (g_xml_use_mem_pool)
	{
		if(strlen(name) > 0 || strlen(value) > 0)
			return m_AttributePool.alloc(name, value);
			
		return m_AttributePool.alloc();
	}
	else
	{
		if(strlen(name) > 0 || strlen(value) > 0)
			return new BiXmlAttribute(name, value);
		
		return new BiXmlAttribute;
	}
}

BiXmlComment* BiXmlFactory::CreateComment()
{
	if (g_xml_use_mem_pool)
		return m_CommentPool.alloc();
	else
		return new BiXmlComment();
}

BiXmlDeclaration* BiXmlFactory::CreateDeclaration(const char* version /* = "" */, const char* encoding /* = "" */, const char* standalone /* = "" */)
{
	if (g_xml_use_mem_pool)
	{
		if (strlen(version) > 0 || strlen(encoding) > 0 || strlen(standalone) > 0)
			return m_DeclarationPool.alloc(version, encoding, standalone);
		
		return m_DeclarationPool.alloc();
	}
	else
	{
		if (strlen(version) > 0 || strlen(encoding) > 0 || strlen(standalone) > 0)
			return new BiXmlDeclaration(version, encoding, standalone);

		return new BiXmlDeclaration();
	}
}

BiXmlDocumentImpl* BiXmlFactory::CreateDocument(const char* name /* = "" */)
{
	if (g_xml_use_mem_pool)
	{
		if (strlen(name) > 0)
			return m_DocumentPool.alloc(name);
		
		return m_DocumentPool.alloc();
	}
	else
	{
		if (strlen(name) > 0)
			return new BiXmlDocumentImpl(name);

		return new BiXmlDocumentImpl();
	}
	
}

BiXmlElementImpl* BiXmlFactory::CreateElement(const char* _value)
{
	if (g_xml_use_mem_pool)
	{
		return m_ElementPool.alloc(_value);
	}
	else
	{
		return new BiXmlElementImpl(_value);
	}
}

BiXmlTextImpl* BiXmlFactory::CreateText(const char* text)
{
	if (g_xml_use_mem_pool)
	{
		return m_TextPool.alloc(text);
	}
	else
	{
		return new BiXmlTextImpl(text);
	}
}

BiXmlUnknown* BiXmlFactory::CreateUnknown()
{
	if (g_xml_use_mem_pool)
	{
		return m_UnknowPool.alloc();
	}
	else
	{
		return new BiXmlUnknown();
	}
}

void BiXmlFactory::ReleaseAttribute(BiXmlAttribute* attr)
{
	if (attr)
	{
		if (g_xml_use_mem_pool)
		{
			m_AttributePool.dealloc(attr);
		}
		else
		{
			delete attr;
		}
		
	}
}
void BiXmlFactory::ReleaseComment(BiXmlComment* com)
{
	if (com)
	{
		if (g_xml_use_mem_pool)
		{
			m_CommentPool.dealloc(com);
		}
		else
		{
			delete com;
		}
		
	}
}
void BiXmlFactory::ReleaseDeclaration(BiXmlDeclaration* decl)
{
	if (decl)
	{
		if (g_xml_use_mem_pool)
		{
			m_DeclarationPool.dealloc(decl);
		}
		else
		{
			delete decl;
		}
		
	}
}
void BiXmlFactory::ReleaseDocument(BiXmlDocumentImpl* doc)
{
	if (doc)
	{
		//delete doc;
		if (g_xml_use_mem_pool)
		{
			m_DocumentPool.dealloc(doc);
		}
		else
		{
			delete doc;
		}
	}
}

void BiXmlFactory::ReleaseElement(BiXmlElementImpl* elem)
{
	if (elem)
	{
		if (g_xml_use_mem_pool)
		{
			m_ElementPool.dealloc(elem);
		}
		else
		{
			delete elem;
		}
	}
}
void BiXmlFactory::ReleaseText(BiXmlTextImpl* text)
{
	if (text)
	{
		if (g_xml_use_mem_pool)
		{
			m_TextPool.dealloc(text);
		}
		else
		{
			delete text;
		}
		
	}
}
void BiXmlFactory::ReleaseUnknown(BiXmlUnknown* unknown)
{
	if (unknown)
	{
		if (g_xml_use_mem_pool)
		{
			m_UnknowPool.dealloc(unknown);
		}
		else
		{
			delete unknown;
		}
		
	}
}

void BiXmlAttribute::Release()
{
	//delete this;
	BiXmlFactory::ReleaseAttribute(this);
}

void BiXmlComment::Release()
{
	//delete this;
	BiXmlFactory::ReleaseComment(this);
}

void BiXmlDeclaration::Release()
{
	//delete this;
	BiXmlFactory::ReleaseDeclaration(this);
}
void BiXmlDocumentImpl::Release()
{
	//delete this;
	BiXmlFactory::ReleaseDocument(this);
}
void BiXmlElementImpl::Release()
{
	//delete this;
	BiXmlFactory::ReleaseElement(this);
}
void BiXmlTextImpl::Release()
{
	//delete this;
	BiXmlFactory::ReleaseText(this);
}
void BiXmlUnknown::Release()
{
	//delete this;
	BiXmlFactory::ReleaseUnknown(this);
}

bool BiXmlBase::condenseWhiteSpace = true;

void BiXmlBase::PutString( const BIXML_STRING& str, BIXML_OSTREAM* stream )
{
	BIXML_STRING buffer;
	PutString( str, &buffer );
	(*stream) << buffer;
}

void BiXmlBase::PutString( const BIXML_STRING& str, BIXML_STRING* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(BIXML_SNPRINTF)		
				BIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


// <-- Strange class for a bug fix. Search for STL_STRING_BUG
BiXmlBase::StringToBuffer::StringToBuffer( const BIXML_STRING& str )
{
	buffer = new char[ str.length()+1 ];
	if ( buffer )
	{
		strcpy( buffer, str.c_str() );
	}
}


BiXmlBase::StringToBuffer::~StringToBuffer()
{
	delete [] buffer;
}
// End strange bug fix. -->


BiXmlNodeImpl::BiXmlNodeImpl( NodeType _type ) : BiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


BiXmlNodeImpl::~BiXmlNodeImpl()
{
	BiXmlNodeImpl* node = firstChild;
	BiXmlNodeImpl* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		//delete temp;
		temp->Release();
	}	
}


void BiXmlNodeImpl::CopyTo( BiXmlNodeImpl* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
}


void BiXmlNodeImpl::Clear()
{
	BiXmlNodeImpl* node = firstChild;
	BiXmlNodeImpl* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		//delete temp;
		temp->Release();
	}	

	firstChild = 0;
	lastChild = 0;
}


BiXmlNodeImpl* BiXmlNodeImpl::_LinkEndChild( BiXmlNodeImpl* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->_GetDocument() == 0 || node->_GetDocument() == this->_GetDocument() );
	
	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


BiXmlNodeImpl* BiXmlNodeImpl::_InsertEndChild( const BiXmlNodeImpl& addThis )
{
	BiXmlNodeImpl* node = addThis._Clone();
	if ( !node )
		return 0;

	return _LinkEndChild( node );
}


BiXmlNodeImpl* BiXmlNodeImpl::InsertBeforeChild( BiXmlNodeImpl* beforeThis, const BiXmlNodeImpl& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this )
		return 0;

	BiXmlNodeImpl* node = addThis._Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


BiXmlNodeImpl* BiXmlNodeImpl::InsertAfterChild( BiXmlNodeImpl* afterThis, const BiXmlNodeImpl& addThis )
{
	if ( !afterThis || afterThis->parent != this )
		return 0;

	BiXmlNodeImpl* node = addThis._Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


BiXmlNodeImpl* BiXmlNodeImpl::ReplaceChild( BiXmlNodeImpl* replaceThis, const BiXmlNodeImpl& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	BiXmlNodeImpl* node = withThis._Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	//delete replaceThis;
	replaceThis->Release();
	node->parent = this;
	return node;
}


bool BiXmlNodeImpl::_RemoveChild( BiXmlNodeImpl* removeThis )
{
	if ( removeThis->parent != this )
	{	
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	//delete removeThis;
	removeThis->Release();
	return true;
}

const BiXmlNodeImpl* BiXmlNodeImpl::_FirstChild( const char * _value ) const
{
	const BiXmlNodeImpl* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


BiXmlNodeImpl* BiXmlNodeImpl::_FirstChild( const char * _value )
{
	BiXmlNodeImpl* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const BiXmlNodeImpl* BiXmlNodeImpl::LastChild( const char * _value ) const
{
	const BiXmlNodeImpl* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

BiXmlNodeImpl* BiXmlNodeImpl::LastChild( const char * _value )
{
	BiXmlNodeImpl* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

const BiXmlNodeImpl* BiXmlNodeImpl::IterateChildren( const BiXmlNodeImpl* previous ) const
{
	if ( !previous )
	{
		return _FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->_NextSibling();
	}
}

BiXmlNodeImpl* BiXmlNodeImpl::IterateChildren( BiXmlNodeImpl* previous )
{
	if ( !previous )
	{
		return _FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->_NextSibling();
	}
}

const BiXmlNode* BiXmlNodeImpl::IterateChildren( const char * value, const BiXmlNode* previous ) const
{
	const BiXmlNodeImpl* pre = dynamic_cast<const BiXmlNodeImpl*>(previous);
	return _IterateChildren(value, pre);
}

BiXmlNode* BiXmlNodeImpl::IterateChildren( const char * value, BiXmlNode* previous )
{
	BiXmlNodeImpl* pre = dynamic_cast<BiXmlNodeImpl*>(previous);
	return _IterateChildren(value, pre);
}

const BiXmlNodeImpl* BiXmlNodeImpl::_IterateChildren( const char * val, const BiXmlNodeImpl* previous ) const
{
	if ( !previous )
	{
		return _FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->_NextSibling( val );
	}
}

BiXmlNodeImpl* BiXmlNodeImpl::_IterateChildren( const char * val, BiXmlNodeImpl* previous )
{
	if ( !previous )
	{
		return _FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->_NextSibling( val );
	}
}

const BiXmlNodeImpl* BiXmlNodeImpl::_NextSibling( const char * _value ) const 
{
	const BiXmlNodeImpl* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

BiXmlNodeImpl* BiXmlNodeImpl::_NextSibling( const char * _value )
{
	BiXmlNodeImpl* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

const BiXmlNodeImpl* BiXmlNodeImpl::PreviousSibling( const char * _value ) const
{
	const BiXmlNodeImpl* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

BiXmlNodeImpl* BiXmlNodeImpl::PreviousSibling( const char * _value )
{
	BiXmlNodeImpl* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

void BiXmlElementImpl::RemoveAttribute( const char * name )
{
	BIXML_STRING str( name );
	BiXmlAttribute* node = attributeSet.Find( str );
	if ( node )
	{
		attributeSet.Remove( node );
		//delete node;
		node->Release();
	}
}

const BiXmlElementImpl* BiXmlNodeImpl::_FirstChildElement() const
{
	const BiXmlNodeImpl* node;

	for (	node = _FirstChild();
			node;
			node = node->_NextSibling() )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

BiXmlElementImpl* BiXmlNodeImpl::_FirstChildElement()
{
	BiXmlNodeImpl* node;

	for (	node = _FirstChild();
			node;
			node = node->_NextSibling() )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

const BiXmlElementImpl* BiXmlNodeImpl::_FirstChildElement( const char * _value ) const
{
	const BiXmlNodeImpl* node;

	for (	node = _FirstChild( _value );
			node;
			node = node->_NextSibling( _value ) )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

BiXmlElementImpl* BiXmlNodeImpl::_FirstChildElement( const char * _value )
{
	BiXmlNodeImpl* node;

	for (	node = _FirstChild( _value );
			node;
			node = node->_NextSibling( _value ) )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

const BiXmlElementImpl* BiXmlNodeImpl::_NextSiblingElement() const
{
	const BiXmlNodeImpl* node;

	for (	node = _NextSibling();
	node;
	node = node->_NextSibling() )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

BiXmlElementImpl* BiXmlNodeImpl::_NextSiblingElement()
{
	BiXmlNodeImpl* node;

	for (	node = _NextSibling();
	node;
	node = node->_NextSibling() )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

const BiXmlElementImpl* BiXmlNodeImpl::_NextSiblingElement( const char * _value ) const
{
	const BiXmlNodeImpl* node;

	for (	node = _NextSibling( _value );
	node;
	node = node->_NextSibling( _value ) )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}

BiXmlElementImpl* BiXmlNodeImpl::_NextSiblingElement( const char * _value )
{
	BiXmlNodeImpl* node;

	for (	node = _NextSibling( _value );
	node;
	node = node->_NextSibling( _value ) )
	{
		if ( node->_ToElement() )
			return node->_ToElement();
	}
	return 0;
}


const BiXmlDocumentImpl* BiXmlNodeImpl::_GetDocument() const
{
	const BiXmlNodeImpl* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}

BiXmlDocumentImpl* BiXmlNodeImpl::_GetDocument()
{
	BiXmlNodeImpl* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


const BiXmlNode* BiXmlNodeImpl::FirstChild( const char * value )const
{
	return _FirstChild(value);
}

BiXmlNode* BiXmlNodeImpl::FirstChild( const char * value )
{
	return _FirstChild(value);
}

BiXmlNode* BiXmlNodeImpl::InsertEndChild( const BiXmlNode& addThis )
{
	const BiXmlNode* pNode = &addThis;
	const BiXmlNodeImpl* pNodeImpl = dynamic_cast<const BiXmlNodeImpl*>(pNode);

	return _InsertEndChild( *pNodeImpl );
}
BiXmlNode* BiXmlNodeImpl::InsertEndChild( const char* strName)
{
	if (!strName)
		return NULL;
	//BiXmlElementImpl* pElem = new BiXmlElementImpl(strName);
	BiXmlElementImpl* pElem = BiXmlCreateElement(strName);

	return _LinkEndChild(pElem);
}

BiXmlNode* BiXmlNodeImpl::LinkEndChild( BiXmlNode* addThis )
{
	return _LinkEndChild(dynamic_cast<BiXmlNodeImpl*>(addThis));
}

const BiXmlNode* BiXmlNodeImpl::NextSibling(const char* value)const
{
	return _NextSibling(value);
}
BiXmlNode* BiXmlNodeImpl::NextSibling(const char* value)
{
	return _NextSibling(value);
}

const BiXmlElement* BiXmlNodeImpl::NextSiblingElement() const
{
	return _NextSiblingElement();
}
BiXmlElement* BiXmlNodeImpl::NextSiblingElement()
{
	return _NextSiblingElement();
}

const BiXmlElement* BiXmlNodeImpl::NextSiblingElement( const char * value) const
{
	return _NextSiblingElement(value);
}
BiXmlElement* BiXmlNodeImpl::NextSiblingElement( const char * value)
{
	return _NextSiblingElement(value);
}

const BiXmlElement* BiXmlNodeImpl::FirstChildElement()	const
{
	return _FirstChildElement();
}
BiXmlElement* BiXmlNodeImpl::FirstChildElement()
{
	return _FirstChildElement();
}

const BiXmlElement* BiXmlNodeImpl::FirstChildElement( const char * value ) const
{
	return _FirstChildElement(value);
}
BiXmlElement* BiXmlNodeImpl::FirstChildElement( const char * value )
{
	return _FirstChildElement(value);
}

bool BiXmlNodeImpl::RemoveChild( BiXmlNode* removeThis )
{
	return _RemoveChild(dynamic_cast<BiXmlNodeImpl*>(removeThis));
}
const BiXmlDocument* BiXmlNodeImpl::GetDocument() const
{
	return _GetDocument();
}
BiXmlDocument* BiXmlNodeImpl::GetDocument()
{
	return _GetDocument();
}

BiXmlElementImpl::BiXmlElementImpl (const char * _value)
	: BiXmlNodeImpl( BiXmlNodeImpl::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}



#ifdef BIXML_USE_STL
BiXmlElementImpl::BiXmlElementImpl( const std::string& _value ) 
	: BiXmlNodeImpl( BiXmlNodeImpl::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


BiXmlElementImpl::BiXmlElementImpl( const BiXmlElementImpl& copy)
	: BiXmlNodeImpl( BiXmlNodeImpl::ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );	
}


void BiXmlElementImpl::operator=( const BiXmlElementImpl& base )
{
	ClearThis();
	base.CopyTo( this );
}


BiXmlElementImpl::~BiXmlElementImpl()
{
	ClearThis();

}


void BiXmlElementImpl::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		BiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		//delete node;
		node->Release();
	}
}


const char * BiXmlElementImpl::Attribute( const char * name ) const
{
	BIXML_STRING str( name );
	const BiXmlAttribute* node = attributeSet.Find( str );

	if ( node )
		return node->Value();

	return 0;
}


const char * BiXmlElementImpl::Attribute( const char * name, int* i ) const
{
	const char * s = Attribute( name );
	if ( i )
	{
		if ( s )
			*i = atoi( s );
		else
			*i = 0;
	}
	return s;
}


const char * BiXmlElementImpl::Attribute( const char * name, double* d ) const
{
	const char * s = Attribute( name );
	if ( d )
	{
		if ( s )
			*d = atof( s );
		else
			*d = 0;
	}
	return s;
}


int BiXmlElementImpl::QueryIntAttribute( const char* name, int* ival ) const
{
	BIXML_STRING str( name );
	const BiXmlAttribute* node = attributeSet.Find( str );
	if ( !node )
		return BIXML_NO_ATTRIBUTE;

	return node->QueryIntValue( ival );
}


int BiXmlElementImpl::QueryDoubleAttribute( const char* name, double* dval ) const
{
	BIXML_STRING str( name );
	const BiXmlAttribute* node = attributeSet.Find( str );
	if ( !node )
		return BIXML_NO_ATTRIBUTE;

	return node->QueryDoubleValue( dval );
}


void BiXmlElementImpl::SetAttribute( const char * name, int val )
{	
	char buf[64];
	#if defined(BIXML_SNPRINTF)		
		BIXML_SNPRINTF( buf, sizeof(buf), "%d", val );
	#else
		sprintf( buf, "%d", val );
	#endif
	SetAttribute( name, buf );
}


#ifdef BIXML_USE_STL
void BiXmlElementImpl::SetAttribute( const std::string& name, int val )
{	
   std::ostringstream oss;
   oss << val;
   SetAttribute( name, oss.str() );
}
#endif


void BiXmlElementImpl::SetDoubleAttribute( const char * name, double val )
{	
	char buf[256];
	#if defined(BIXML_SNPRINTF)		
		BIXML_SNPRINTF( buf, sizeof(buf), "%f", val );
	#else
		sprintf( buf, "%f", val );
	#endif
	SetAttribute( name, buf );
}


void BiXmlElementImpl::SetAttribute( const char * cname, const char * cvalue )
{
	BIXML_STRING _name( cname );
	BIXML_STRING _value( cvalue );

	BiXmlAttribute* node = attributeSet.Find( _name );
	if ( node )
	{
		node->SetValue( cvalue );
		return;
	}

	//BiXmlAttribute* attrib = new BiXmlAttribute( cname, cvalue );
	BiXmlAttribute* attrib = BiXmlCreateAttribute(cname, cvalue);
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		BiXmlDocumentImpl* document = _GetDocument();
		if ( document ) document->SetError( BIXML_ERROR_OUT_OF_MEMORY, 0, 0, BIXML_ENCODING_UNKNOWN );
	}
}


#ifdef BIXML_USE_STL
void BiXmlElementImpl::SetAttribute( const std::string& name, const std::string& _value )
{
	BiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	//BiXmlAttribute* attrib = new BiXmlAttribute( name, _value );
	BiXmlAttribute* attrib = BiXmlCreateAttribute( name.c_str(), _value.c_str());
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		BiXmlDocumentImpl* document = _GetDocument();
		if ( document ) document->SetError( BIXML_ERROR_OUT_OF_MEMORY, 0, 0, BIXML_ENCODING_UNKNOWN );
	}
}
#endif


void BiXmlElementImpl::Print( FILE* cfile, int depth ) const
{
	int i;
	for ( i=0; i<depth; i++ )
	{
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const BiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	BiXmlNodeImpl* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->_ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->_NextSibling() )
		{
			if ( !node->_ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i )
		fprintf( cfile, "    " );
		fprintf( cfile, "</%s>", value.c_str() );
	}
}

void BiXmlElementImpl::StreamOut( BIXML_OSTREAM * stream ) const
{
	(*stream) << "<" << value;

	const BiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{	
		(*stream) << " ";
		attrib->StreamOut( stream );
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	BiXmlNodeImpl* node;
	if ( firstChild )
	{ 		
		(*stream) << ">";

		for ( node = firstChild; node; node=node->_NextSibling() )
		{
			node->StreamOut( stream );
		}
		(*stream) << "</" << value << ">";
	}
	else
	{
		(*stream) << " />";
	}
}


void BiXmlElementImpl::CopyTo( BiXmlElementImpl* target ) const
{
	// superclass:
	BiXmlNodeImpl::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	const BiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	BiXmlNodeImpl* node = 0;
	for ( node = firstChild; node; node = node->_NextSibling() )
	{
		target->_LinkEndChild( node->_Clone() );
	}
}


BiXmlNodeImpl* BiXmlElementImpl::_Clone() const
{
	//BiXmlElementImpl* clone = new BiXmlElementImpl( Value() );
	BiXmlElementImpl* clone = BiXmlCreateElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* BiXmlElementImpl::GetText() const
{
	const BiXmlNodeImpl* child = this->_FirstChild();
	if ( child ) {
		const BiXmlTextImpl* childText = child->_ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}

bool BiXmlElementImpl::SetText(const char* strText)
{
	bool bRet = false;
	if (!strText)
		return bRet;

	BiXmlNodeImpl* child = this->_FirstChild();
	if ( child ) {
		BiXmlTextImpl* childText = child->_ToText();
		if ( childText ) {
			childText->SetValue(strText);
			bRet = true;
		}
	}
	return bRet;
}

bool BiXmlElementImpl::AddText(const char* strText)
{
	bool bRet = false;
	//BiXmlTextImpl* pText = new BiXmlTextImpl(strText);
	BiXmlTextImpl* pText = BiXmlCreateText(strText);
	if ( !pText )
		return bRet;

	bRet = _LinkEndChild( pText ) ? true : false;

	return bRet;
}

bool BiXmlDocumentImpl::InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone)
{
	return _InsertEndChild(BiXmlDeclaration(_version, _encoding, _standalone))? true:false;
}

const char* BiXmlDocumentImpl::ParseXmlStr(const char* strContent)
{
	return _Parse(strContent);
}

const char* BiXmlDocumentImpl::Parse(const char* strContent)
{
	return _Parse(strContent);
}

bool BiXmlDocumentImpl::Error()const
{
	return _Error();
}
BiXmlDocumentImpl::BiXmlDocumentImpl() : BiXmlNodeImpl( BiXmlNodeImpl::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

BiXmlDocumentImpl::BiXmlDocumentImpl( const char * documentName ) : BiXmlNodeImpl( BiXmlNodeImpl::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef BIXML_USE_STL
BiXmlDocumentImpl::BiXmlDocumentImpl( const std::string& documentName ) : BiXmlNodeImpl( BiXmlNodeImpl::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


BiXmlDocumentImpl::BiXmlDocumentImpl( const BiXmlDocumentImpl& copy ) : BiXmlNodeImpl( BiXmlNodeImpl::DOCUMENT )
{
	copy.CopyTo( this );
}


void BiXmlDocumentImpl::operator=( const BiXmlDocumentImpl& copy )
{
	Clear();
	copy.CopyTo( this );
}


bool BiXmlDocumentImpl::LoadFile( BiXmlEncoding encoding )
{
	// See STL_STRING_BUG below.
	StringToBuffer buf( value );

	if ( buf.buffer && LoadFile( buf.buffer, encoding ) )
		return true;

	return false;
}


//bool BiXmlDocumentImpl::SaveFile() const
bool BiXmlDocumentImpl::SaveFile()
{
	// See STL_STRING_BUG below.
	StringToBuffer buf( value );

	if ( buf.buffer && SaveFile( buf.buffer ) )
		return true;

	return false;
}

bool BiXmlDocumentImpl::LoadFile( const char* filename, BiXmlEncoding encoding )
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// See STL_STRING_BUG above.
	// Fixed with the StringToBuffer class.
	value = filename;

	//add by puzzy
	if (LoadFileFromPack(value.c_str(),encoding))
	{
		//H3D::TimingProfile::GetSingleton().XMLLoadEnd();
		return true;
	}

	//!add by puzzy
	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = fopen( value.c_str (), "rb" );	

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( BIXML_ERROR_OPENING_FILE, 0, 0, BIXML_ENCODING_UNKNOWN );
		return false;
	}
}

bool BiXmlDocumentImpl::LoadFileFromPack(const char* filename,BiXmlEncoding encoding )
{
	if (strstr(filename,"ogg"))
	{
		int a=0;
	}
	VFS::IFile* file = VFS::Hfopen(filename,"rb",0);
	if ( file)
	{
		// Get the file size, so we can pre-allocate the string. HUGE speed impact.
		long length = (long)file->GetSize();

		// Strange case, but good to handle up front.
		if ( length == 0 )
		{
			VFS::Hfclose( file );
			return false;
		}

		// If we have a file, assume it is all one big XML file, and read it in.
		// The document parser may decide the document ends sooner than the entire file, however.
		BIXML_STRING data;
		data.reserve( length );

		//const int BUF_SIZE = 2048;
		//char buf[BUF_SIZE];

		//int nsize = 0;
		//FILE* f = file->GetCFileHandle();
		//int filesize = (int)file->GetSize();
		//while (nsize<filesize)
		//{
		//	fgets(buf,BUF_SIZE,f);
		//	nsize += (int)strlen(buf);
		//	data +=buf;
		//}
		char* buf = new char[ length+1 ];
		buf[0] = 0;

		if ( VFS::Hfread( buf, length, 1, file ) != 1 ) {
			SetError( BIXML_ERROR_OPENING_FILE, 0, 0, BIXML_ENCODING_UNKNOWN );
			VFS::Hfclose(file);
			return false;
		}

		const char* lastPos = buf;
		const char* p = buf;

		buf[length] = 0;
		while( *p ) {
			assert( p < (buf+length) );
			if ( *p == 0xa ) {
				// Newline character. No special rules for this. Append all the characters
				// since the last string, and include the newline.
				data.append( lastPos, (p-lastPos+1) );	// append, include the newline
				++p;									// move past the newline
				lastPos = p;							// and point to the new buffer (may be 0)
				assert( p <= (buf+length) );
			}
			else if ( *p == 0xd ) {
				// Carriage return. Append what we have so far, then
				// handle moving forward in the buffer.
				if ( (p-lastPos) > 0 ) {
					data.append( lastPos, p-lastPos );	// do not add the CR
				}
				data += (char)0xa;						// a proper newline

				if ( *(p+1) == 0xa ) {
					// Carriage return - new line sequence
					p += 2;
					lastPos = p;
					assert( p <= (buf+length) );
				}
				else {
					// it was followed by something else...that is presumably characters again.
					++p;
					lastPos = p;
					assert( p <= (buf+length) );
				}
			}
			else {
				++p;
			}
		}
		// Handle any left over characters.
		if ( p-lastPos ) {
			data.append( lastPos, p-lastPos );
		}		
		delete [] buf;
		buf = 0;
		VFS::Hfclose(file);


		_Parse( data.c_str(), 0, encoding );

		if (  _Error() )
			return false;
		else
			return true;
	}
//	SetError( BIXML_ERROR_OPENING_FILE, 0, 0, BIXML_ENCODING_UNKNOWN );

	return false;
}
bool BiXmlDocumentImpl::LoadFile( FILE* file, BiXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( BIXML_ERROR_OPENING_FILE, 0, 0, BIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length == 0 )
	{
		SetError( BIXML_ERROR_DOCUMENT_EMPTY, 0, 0, BIXML_ENCODING_UNKNOWN );
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	BIXML_STRING data;
	data.reserve( length );

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		SetError( BIXML_ERROR_OPENING_FILE, 0, 0, BIXML_ENCODING_UNKNOWN );
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		if ( *p == 0xa ) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd ) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa ) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos ) {
		data.append( lastPos, p-lastPos );
	}		
	delete [] buf;
	buf = 0;

	_Parse( data.c_str(), 0, encoding );

	if (  _Error() )
        return false;
    else
		return true;
}


//bool BiXmlDocumentImpl::SaveFile( const char * filename ) const
bool BiXmlDocumentImpl::SaveFile( const char * filename )
{
	// The old c stuff lives on...
	FILE* fp = fopen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


bool BiXmlDocumentImpl::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char BIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char BIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char BIXML_UTF_LEAD_2 = 0xbfU;

		fputc( BIXML_UTF_LEAD_0, fp );
		fputc( BIXML_UTF_LEAD_1, fp );
		fputc( BIXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return true;
}


void BiXmlDocumentImpl::CopyTo( BiXmlDocumentImpl* target ) const
{
	BiXmlNodeImpl::CopyTo( target );

	target->error = error;
	target->errorDesc = errorDesc.c_str ();

	BiXmlNodeImpl* node = 0;
	for ( node = firstChild; node; node = node->_NextSibling() )
	{
		target->_LinkEndChild( node->_Clone() );
	}	
}


BiXmlNodeImpl* BiXmlDocumentImpl::_Clone() const
{
	//BiXmlDocumentImpl* clone = new BiXmlDocumentImpl();
	BiXmlDocumentImpl* clone = BiXmlCreateDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void BiXmlDocumentImpl::Print( FILE* cfile, int depth ) const
{
	const BiXmlNodeImpl* node;
	for ( node=_FirstChild(); node; node=node->_NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}

void BiXmlDocumentImpl::StreamOut( BIXML_OSTREAM * out ) const
{
	const BiXmlNodeImpl* node;
	for ( node=_FirstChild(); node; node=node->_NextSibling() )
	{
		node->StreamOut( out );

		// Special rule for streams: stop after the root element.
		// The stream in code will only read one element, so don't
		// write more than one.
		if ( node->ToElement() )
			break;
	}
}


const BiXmlAttribute* BiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

BiXmlAttribute* BiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

const BiXmlAttribute* BiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

BiXmlAttribute* BiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

void BiXmlAttribute::Print( FILE* cfile, int /*depth*/ ) const
{
	BIXML_STRING n, v;

	PutString( name, &n );
	PutString( value, &v );

	if (value.find ('\"') == BIXML_STRING::npos)
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
	else
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
}


void BiXmlAttribute::StreamOut( BIXML_OSTREAM * stream ) const
{
	if (value.find( '\"' ) != BIXML_STRING::npos)
	{
		PutString( name, stream );
		(*stream) << "=" << "'";
		PutString( value, stream );
		(*stream) << "'";
	}
	else
	{
		PutString( name, stream );
		(*stream) << "=" << "\"";
		PutString( value, stream );
		(*stream) << "\"";
	}
}

int BiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( sscanf( value.c_str(), "%d", ival ) == 1 )
		return BIXML_SUCCESS;
	return BIXML_WRONG_TYPE;
}

int BiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( sscanf( value.c_str(), "%lf", dval ) == 1 )
		return BIXML_SUCCESS;
	return BIXML_WRONG_TYPE;
}

void BiXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(BIXML_SNPRINTF)		
		BIXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void BiXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(BIXML_SNPRINTF)		
		BIXML_SNPRINTF( buf, sizeof(buf), "%lf", _value);
	#else
		sprintf (buf, "%lf", _value);
	#endif
	SetValue (buf);
}

int BiXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}

double  BiXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}


BiXmlComment::BiXmlComment( const BiXmlComment& copy ) : BiXmlNodeImpl( BiXmlNodeImpl::COMMENT )
{
	copy.CopyTo( this );
}


void BiXmlComment::operator=( const BiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}


void BiXmlComment::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
	{
		fputs( "    ", cfile );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}

void BiXmlComment::StreamOut( BIXML_OSTREAM * stream ) const
{
	(*stream) << "<!--";
	//PutString( value, stream );
	(*stream) << value;
	(*stream) << "-->";
}


void BiXmlComment::CopyTo( BiXmlComment* target ) const
{
	BiXmlNodeImpl::CopyTo( target );
}


BiXmlNodeImpl* BiXmlComment::_Clone() const
{
	//BiXmlComment* clone = new BiXmlComment();
	BiXmlComment* clone = BiXmlCreateComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void BiXmlTextImpl::Print( FILE* cfile, int depth ) const
{
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[" );
		fprintf( cfile, "%s", value.c_str() );	// unformatted output
		fprintf( cfile, "]]>\n" );
	}
	else
	{
		BIXML_STRING buffer;
		PutString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void BiXmlTextImpl::StreamOut( BIXML_OSTREAM * stream ) const
{
	if ( cdata )
	{
		(*stream) << "<![CDATA[" << value << "]]>";
	}
	else
	{
		PutString( value, stream );
	}
}


void BiXmlTextImpl::CopyTo( BiXmlTextImpl* target ) const
{
	BiXmlNodeImpl::CopyTo( target );
	target->cdata = cdata;
}


BiXmlNodeImpl* BiXmlTextImpl::_Clone() const
{	
	BiXmlTextImpl* clone = 0;
	//clone = new BiXmlTextImpl( "" );
	clone = BiXmlCreateText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


BiXmlDeclaration::BiXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: BiXmlNodeImpl( BiXmlNodeImpl::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef BIXML_USE_STL
BiXmlDeclaration::BiXmlDeclaration(	const std::string& _version,
									const std::string& _encoding,
									const std::string& _standalone )
	: BiXmlNodeImpl( BiXmlNodeImpl::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


BiXmlDeclaration::BiXmlDeclaration( const BiXmlDeclaration& copy )
	: BiXmlNodeImpl( BiXmlNodeImpl::DECLARATION )
{
	copy.CopyTo( this );	
}


void BiXmlDeclaration::operator=( const BiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}


void BiXmlDeclaration::Print( FILE* cfile, int /*depth*/ ) const
{
	fprintf (cfile, "<?xml ");

	if ( !version.empty() )
		fprintf (cfile, "version=\"%s\" ", version.c_str ());
	if ( !encoding.empty() )
		fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
	if ( !standalone.empty() )
		fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
	fprintf (cfile, "?>");
}

void BiXmlDeclaration::StreamOut( BIXML_OSTREAM * stream ) const
{
	(*stream) << "<?xml ";

	if ( !version.empty() )
	{
		(*stream) << "version=\"";
		PutString( version, stream );
		(*stream) << "\" ";
	}
	if ( !encoding.empty() )
	{
		(*stream) << "encoding=\"";
		PutString( encoding, stream );
		(*stream ) << "\" ";
	}
	if ( !standalone.empty() )
	{
		(*stream) << "standalone=\"";
		PutString( standalone, stream );
		(*stream) << "\" ";
	}
	(*stream) << "?>";
}


void BiXmlDeclaration::CopyTo( BiXmlDeclaration* target ) const
{
	BiXmlNodeImpl::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


BiXmlNodeImpl* BiXmlDeclaration::_Clone() const
{	
	//BiXmlDeclaration* clone = new BiXmlDeclaration();
	BiXmlDeclaration* clone = BiXmlCreateDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void BiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void BiXmlUnknown::StreamOut( BIXML_OSTREAM * stream ) const
{
	(*stream) << "<" << value << ">";		// Don't use entities here! It is unknown.
}


void BiXmlUnknown::CopyTo( BiXmlUnknown* target ) const
{
	BiXmlNodeImpl::CopyTo( target );
}


BiXmlNodeImpl* BiXmlUnknown::_Clone() const
{
	//BiXmlUnknown* clone = new BiXmlUnknown();
	BiXmlUnknown* clone = BiXmlCreateUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


BiXmlAttributeSet::BiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


BiXmlAttributeSet::~BiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void BiXmlAttributeSet::Add( BiXmlAttribute* addMe )
{
	assert( !Find( BIXML_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void BiXmlAttributeSet::Remove( BiXmlAttribute* removeMe )
{
	BiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}

const BiXmlAttribute* BiXmlAttributeSet::Find( const BIXML_STRING& name ) const
{
	const BiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

BiXmlAttribute*	BiXmlAttributeSet::Find( const BIXML_STRING& name )
{
	BiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

#ifdef BIXML_USE_STL	
BIXML_ISTREAM & operator >> (BIXML_ISTREAM & in, BiXmlNodeImpl & base)
{
	BIXML_STRING tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base._Parse( tag.c_str(), 0, BIXML_DEFAULT_ENCODING );
	return in;
}
#endif


BIXML_OSTREAM & operator<< (BIXML_OSTREAM & out, const BiXmlNodeImpl & base)
{
	base.StreamOut (& out);
	return out;
}


#ifdef BIXML_USE_STL	
std::string & operator<< (std::string& out, const BiXmlNodeImpl& base )
{
   std::ostringstream os_stream( std::ostringstream::out );
   base.StreamOut( &os_stream );
   
   out.append( os_stream.str() );
   return out;
}
#endif


BiXmlHandle BiXmlHandle::FirstChild() const
{
	if ( node )
	{
		BiXmlNodeImpl* child = node->_FirstChild();
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		BiXmlNodeImpl* child = node->_FirstChild( value );
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		BiXmlElementImpl* child = node->_FirstChildElement();
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		BiXmlElementImpl* child = node->_FirstChildElement( value );
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		BiXmlNodeImpl* child = node->_FirstChild();
		for (	i=0;
				child && i<count;
				child = child->_NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		BiXmlNodeImpl* child = node->_FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->_NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		BiXmlElementImpl* child = node->_FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->_NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}


BiXmlHandle BiXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		BiXmlElementImpl* child = node->_FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->_NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return BiXmlHandle( child );
	}
	return BiXmlHandle( 0 );
}
