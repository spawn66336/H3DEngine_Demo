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


#ifndef BINYXML_INCLUDED
#define BINYXML_INCLUDED

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IXMLNode.h"
#include "../MemAllocator.h"

// Help out windows:
#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif
////add by sssa2000 090430
//#define BIXML_USE_STL 1

#ifdef BIXML_USE_STL
	#include <string>
 	#include <iostream>
	#define BIXML_STRING	std::string
	#define BIXML_ISTREAM	std::istream
	#define BIXML_OSTREAM	std::ostream
#else
	#include "tinystr.h"
	#define BIXML_STRING	BiXmlString
	#define BIXML_OSTREAM	BiXmlOutStream
#endif

// Deprecated library function hell. Compilers want to use the
// new safe versions. This probably doesn't fully address the problem,
// but it gets closer. There are too many compilers for me to fully
// test. If you get compilation troubles, undefine BIXML_SAFE

#define BIXML_SAFE		// TinyXml isn't fully buffer overrun protected, safe code. This is work in progress.
#ifdef BIXML_SAFE
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		// Microsoft visual studio, version 2005 and higher.
		#define BIXML_SNPRINTF _snprintf_s
		#define BIXML_SNSCANF  _snscanf_s
	#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
		// Microsoft visual studio, version 6 and higher.
		//#pragma message( "Using _sn* functions." )
		#define BIXML_SNPRINTF _snprintf
		#define BIXML_SNSCANF  _snscanf
	#elif defined(__GNUC__) && (__GNUC__ >= 3 )
		// GCC version 3 and higher.s
		//#warning( "Using sn* functions." )
		#define BIXML_SNPRINTF snprintf
		#define BIXML_SNSCANF  snscanf
	#endif
#endif	

class BiXmlDocumentImpl;
class BiXmlElementImpl;
class BiXmlComment;
class BiXmlUnknown;
class BiXmlAttribute;
class BiXmlTextImpl;
class BiXmlDeclaration;
class BiXmlParsingData;

const int BIXML_MAJOR_VERSION = 2;
const int BIXML_MINOR_VERSION = 4;
const int BIXML_PATCH_VERSION = 3;

/*	Internal structure for tracking location of items 
	in the XML file.
*/
struct BiXmlCursor
{
	BiXmlCursor()		{ Clear(); }
	void Clear()		{ row = col = -1; }

	int row;	// 0 based.
	int col;	// 0 based.
};


// Only used by Attribute::Query functions
enum 
{ 
	BIXML_SUCCESS,
	BIXML_NO_ATTRIBUTE,
	BIXML_WRONG_TYPE
};


// Used by the parsing routines.
//enum BiXmlEncoding
//{
//	BIXML_ENCODING_UNKNOWN,
//	BIXML_ENCODING_UTF8,
//	BIXML_ENCODING_LEGACY
//};

const BiXmlEncoding BIXML_DEFAULT_ENCODING = BIXML_ENCODING_UNKNOWN;

//属性
BiXmlAttribute*		BiXmlCreateAttribute(const char* name = "", const char* value = "");
void				BiXmlReleaseAttribute(BiXmlAttribute* attr);
//解释
BiXmlComment*		BiXmlCreateComment();
//void				BiXmlReleaseComment(BiXmlComment* comment);
//声明
BiXmlDeclaration*	BiXmlCreateDeclaration(const char* version = "", const char* encoding = "", const char* standalone = "");
//void				BiXmlReleaseDeclaration(BiXmlDeclaration* decl);
//文档
BiXmlDocumentImpl*	BiXmlCreateDocument(const char* name = "");
void				BiXmlReleaseDocument(BiXmlDocumentImpl*	doc);
//元素
BiXmlElementImpl*	BiXmlCreateElement(const char* _value);
void				BiXmlReleaseElement(BiXmlElementImpl* elem);
//文本
BiXmlTextImpl*			BiXmlCreateText(const char* text);
//void				BiXmlReleaseText(BiXmlTextImpl* text);
//未知
BiXmlUnknown*		BiXmlCreateUnknown();
//void				BiXmlReleaseUnknown(BiXmlUnknown* unknown);

class BiXmlFactory
{
public:
	static BiXmlAttribute*		CreateAttribute(const char* name = "", const char* value = "");
	static BiXmlComment*		CreateComment();
	static BiXmlDeclaration*	CreateDeclaration(const char* version = "", const char* encoding = "", const char* standalone = "");
	static BiXmlDocumentImpl*	CreateDocument(const char* name = "");
	static BiXmlElementImpl*	CreateElement(const char* _value);
	static BiXmlTextImpl*			CreateText(const char* text);
	static BiXmlUnknown*		CreateUnknown();

	static void					ReleaseAttribute(BiXmlAttribute* attr);
	static void					ReleaseComment(BiXmlComment* com);
	static void					ReleaseDeclaration(BiXmlDeclaration* decl);
	static void					ReleaseDocument(BiXmlDocumentImpl* doc);
	static void					ReleaseElement(BiXmlElementImpl* elem);
	static void					ReleaseText(BiXmlTextImpl* text);
	static void					ReleaseUnknown(BiXmlUnknown* unknown);

private:
	static ClassPool<BiXmlAttribute, CBiboLock>		m_AttributePool;
	static ClassPool<BiXmlComment, CBiboLock>		m_CommentPool;
	static ClassPool<BiXmlDeclaration, CBiboLock>	m_DeclarationPool;
	static ClassPool<BiXmlDocumentImpl, CBiboLock>	m_DocumentPool;
	static ClassPool<BiXmlElementImpl, CBiboLock>	m_ElementPool;
	static ClassPool<BiXmlTextImpl, CBiboLock>			m_TextPool;
	static ClassPool<BiXmlUnknown, CBiboLock>		m_UnknowPool;
};

#define BIXML_PUBLIC_INHERIT \
	virtual void Clear(){return BiXmlNodeImpl::Clear();}\
	virtual const BiXmlNode* FirstChild()const{	return BiXmlNodeImpl::FirstChild();}\
	virtual BiXmlNode* FirstChild(){ return BiXmlNodeImpl::FirstChild();}\
	virtual const BiXmlNode* FirstChild( const char * value ) const{return BiXmlNodeImpl::FirstChild(value);}\
	virtual BiXmlNode* FirstChild( const char * value ) {return BiXmlNodeImpl::FirstChild(value);}\
	virtual const BiXmlElement* FirstChildElement()	const{return BiXmlNodeImpl::FirstChildElement();}\
	virtual BiXmlElement* FirstChildElement(){return BiXmlNodeImpl::FirstChildElement();}\
	virtual const BiXmlElement* FirstChildElement( const char * value ) const{return BiXmlNodeImpl::FirstChildElement(value);}\
	virtual BiXmlElement* FirstChildElement( const char * value ){return BiXmlNodeImpl::FirstChildElement(value);}\
	virtual const BiXmlNode* NextSibling() const{return BiXmlNodeImpl::NextSibling();}\
	virtual BiXmlNode* NextSibling(){return BiXmlNodeImpl::NextSibling();}\
	virtual const BiXmlNode* NextSibling( const char * _value) const{return BiXmlNodeImpl::NextSibling(_value);}\
	virtual BiXmlNode* NextSibling( const char * _value){return BiXmlNodeImpl::NextSibling(_value);}\
	virtual const BiXmlElement* NextSiblingElement() const{	return BiXmlNodeImpl::NextSiblingElement();}\
	virtual BiXmlElement* NextSiblingElement(){	return BiXmlNodeImpl::NextSiblingElement();}\
	virtual const BiXmlElement* NextSiblingElement( const char * _value) const{	return BiXmlNodeImpl::NextSiblingElement(_value);}\
	virtual BiXmlElement* NextSiblingElement( const char * _value){	return BiXmlNodeImpl::NextSiblingElement(_value);}\
	virtual const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const{return BiXmlNodeImpl::IterateChildren(value, previous);}\
	virtual BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous ){return BiXmlNodeImpl::IterateChildren(value, previous);}\
	virtual const BiXmlDocument* GetDocument() const{return BiXmlNodeImpl::GetDocument();}\
	virtual BiXmlDocument* GetDocument(){return BiXmlNodeImpl::GetDocument();}\
	virtual const BiXmlNode* Parent() const{return BiXmlNodeImpl::Parent();}\
	virtual BiXmlNode* InsertEndChild( const BiXmlNode& addThis ){return BiXmlNodeImpl::InsertEndChild(addThis);}\
	virtual BiXmlNode* InsertEndChild( const char* strName ){return BiXmlNodeImpl::InsertEndChild(strName);}\
	virtual BiXmlNode* LinkEndChild( BiXmlNode* addThis ){return BiXmlNodeImpl::LinkEndChild(addThis);}\
	virtual bool RemoveChild( BiXmlNode* removeThis ){return BiXmlNodeImpl::RemoveChild(removeThis);};\
	virtual void SetValue(const char * _value){	return BiXmlNodeImpl::SetValue(_value);}\
	virtual const char * Value() const{return BiXmlNodeImpl::Value();}\
	virtual DocType GetXmlType(){return DT_XML;}

/** BiXmlBase is a base class for every class in TinyXml.
	It does little except to establish that TinyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class BiXmlBase
{
	friend class BiXmlNodeImpl;
	friend class BiXmlElementImpl;
	friend class BiXmlDocumentImpl;
	friend class BiXmlFactory;

protected:

	BiXmlBase()	:	userData(0) {}
	virtual ~BiXmlBase()					{}
public:

	virtual void Release() = 0;
	/**	All TinyXml classes can print themselves to a filestream.
		This is a formatted print, and will insert tabs and newlines.
		
		(For an unformatted stream, use the << operator.)
	*/
	virtual void Print( FILE* cfile, int depth ) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not TinyXml will condense all white space
		into a single space or not. The default is to condense. Note changing this
		values is not thread safe.
	*/
	static void SetCondenseWhiteSpace( bool condense )		{ condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the BiXmlDocumentImpl::Load(),
		BiXmlDocumentImpl::LoadFile(), or any BiXmlNode::Parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if BiXmlDocumentImpl::SetTabSize() is called with 0 as the value.

		@sa BiXmlDocumentImpl::SetTabSize()
	*/
	int Row() const			{ return location.row + 1; }
	int Column() const		{ return location.col + 1; }	///< See Row()

	void  SetUserData( void* user )			{ userData = user; }
	void* GetUserData()						{ return userData; }

	// Table that returs, for a given lead byte, the total number of bytes
	// in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* _Parse(	const char* p, 
								BiXmlParsingData* data, 
								BiXmlEncoding encoding /*= BIXML_ENCODING_UNKNOWN */ ) = 0;

	enum
	{
		BIXML_NO_ERROR = 0,
		BIXML_ERROR,
		BIXML_ERROR_OPENING_FILE,
		BIXML_ERROR_OUT_OF_MEMORY,
		BIXML_ERROR_PARSING_ELEMENT,
		BIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		BIXML_ERROR_READING_ELEMENT_VALUE,
		BIXML_ERROR_READING_ATTRIBUTES,
		BIXML_ERROR_PARSING_EMPTY,
		BIXML_ERROR_READING_END_TAG,
		BIXML_ERROR_PARSING_UNKNOWN,
		BIXML_ERROR_PARSING_COMMENT,
		BIXML_ERROR_PARSING_DECLARATION,
		BIXML_ERROR_DOCUMENT_EMPTY,
		BIXML_ERROR_EMBEDDED_NULL,
		BIXML_ERROR_PARSING_CDATA,

		BIXML_ERROR_STRING_COUNT
	};

protected:

	// See STL_STRING_BUG
	// Utility class to overcome a bug.
	class StringToBuffer
	{
	  public:
		StringToBuffer( const BIXML_STRING& str );
		~StringToBuffer();
		char* buffer;
	};

	static const char*	SkipWhiteSpace( const char*, BiXmlEncoding encoding );
	inline static bool	IsWhiteSpace( char c )		
	{ 
		return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
	}
	inline static bool	IsWhiteSpace( int c )
	{
		if ( c < 256 )
			return IsWhiteSpace( (char) c );
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}
public:
	virtual void StreamOut (BIXML_OSTREAM *) const = 0;
protected:

	#ifdef BIXML_USE_STL
	    static bool	StreamWhiteSpace( BIXML_ISTREAM * in, BIXML_STRING * tag );
	    static bool StreamTo( BIXML_ISTREAM * in, int character, BIXML_STRING * tag );
	#endif

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* ReadName( const char* p, BIXML_STRING* name, BiXmlEncoding encoding );

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* ReadText(	const char* in,				// where to start
									BIXML_STRING* text,			// the string read
									bool ignoreWhiteSpace,		// whether to keep the white space
									const char* endTag,			// what ends this text
									bool ignoreCase,			// whether to ignore case in the end tag
									BiXmlEncoding encoding );	// the current encoding

	// If an entity has been found, transform it into a character.
	static const char* GetEntity( const char* in, char* value, int* length, BiXmlEncoding encoding );

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* GetChar( const char* p, char* _value, int* length, BiXmlEncoding encoding )
	{
		assert( p );
		if ( encoding == BIXML_ENCODING_UTF8 )
		{
			*length = utf8ByteTable[ *((unsigned char*)p) ];
			assert( *length >= 0 && *length < 5 );
		}
		else
		{
			*length = 1;
		}

		if ( *length == 1 )
		{
			if ( *p == '&' )
				return GetEntity( p, _value, length, encoding );
			*_value = *p;
			return p+1;
		}
		else if ( *length )
		{
			//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
												// and the null terminator isn't needed
			for( int i=0; p[i] && i<*length; ++i ) {
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Puts a string to a stream, expanding entities as it goes.
	// Note this should not contian the '<', '>', etc, or they will be transformed into entities!
	static void PutString( const BIXML_STRING& str, BIXML_OSTREAM* out );

	static void PutString( const BIXML_STRING& str, BIXML_STRING* out );

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool StringEqual(	const char* p,
								const char* endTag,
								bool ignoreCase,
								BiXmlEncoding encoding );

	static const char* errorString[ BIXML_ERROR_STRING_COUNT ];

	BiXmlCursor location;

    /// Field containing a generic user pointer
	void*			userData;
	
	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int IsAlpha( unsigned char anyByte, BiXmlEncoding encoding );
	static int IsAlphaNum( unsigned char anyByte, BiXmlEncoding encoding );
	inline static int ToLower( int v, BiXmlEncoding encoding )
	{
		if ( encoding == BIXML_ENCODING_UTF8 )
		{
			if ( v < 128 ) return tolower( v );
			return v;
		}
		else
		{
			return tolower( v );
		}
	}
	static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

private:
	BiXmlBase( const BiXmlBase& );				// not implemented.
	void operator=( const BiXmlBase& base );	// not allowed.

	struct Entity
	{
		const char*     str;
		unsigned int	strLength;
		char		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a BiXmlNodeImpl
	can be queried, and it can be cast to its more defined type.
*/
class BiXmlNodeImpl : public BiXmlBase, public BiXmlNode
{
public:
	//virtual void Clear();
private:
	friend class BiXmlDocumentImpl;
	friend class BiXmlElementImpl;

public:
	#ifdef BIXML_USE_STL	

	    /** An input stream operator, for every class. Tolerant of newlines and
		    formatting, but doesn't expect them.
	    */
	    friend std::istream& operator >> (std::istream& in, BiXmlNode& base);

	    /** An output stream operator, for every class. Note that this outputs
		    without any newlines or formatting, as opposed to Print(), which
		    includes tabs and new lines.

		    The operator<< and operator>> are not completely symmetric. Writing
		    a node to a stream is very well defined. You'll get a nice stream
		    of output, without any extra whitespace or newlines.
		    
		    But reading is not as well defined. (As it always is.) If you create
		    a BiXmlElementImpl (for example) and read that from an input stream,
		    the text needs to define an element or junk will result. This is
		    true of all input streams, but it's worth keeping in mind.

		    A BiXmlDocumentImpl will read nodes until it reads a root element, and
			all the children of that root element.
	    */	
	    friend std::ostream& operator<< (std::ostream& out, const BiXmlNode& base);

		/// Appends the XML node or attribute to a std::string.
		friend std::string& operator<< (std::string& out, const BiXmlNode& base );

	#else
	    // Used internally, not part of the public API.
	    friend BIXML_OSTREAM& operator<< (BIXML_OSTREAM& out, const BiXmlNodeImpl& base);
	#endif

	/** The types of XML nodes supported by TinyXml. (All the
			unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	virtual ~BiXmlNodeImpl();

	/** The meaning of 'value' changes for the specific type of
		BiXmlNodeImpl.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const char *Value() const { return value.c_str (); }

    #ifdef BIXML_USE_STL
	/** Return Value() as a std::string. If you only use STL,
	    this is more efficient than calling Value().
		Only available in STL mode.
	*/
	const std::string& ValueStr() const { return value; }
	#endif

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void SetValue(const char * _value) { value = _value;}

    #ifdef BIXML_USE_STL
	/// STL std::string form.
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	BiXmlNode* Parent()							{ return parent; }
	const BiXmlNode* Parent() const				{ return parent; }

	const BiXmlNode* FirstChild()	const	{ return firstChild; }		///< The first child of this node. Will be null if there are no children.
	BiXmlNode* FirstChild()					{ return firstChild; }
	const BiXmlNode* FirstChild( const char * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	BiXmlNode* FirstChild( const char * value );						///< The first child of this node with the matching 'value'. Will be null if none found.

	const BiXmlNodeImpl* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	BiXmlNodeImpl* LastChild()	{ return lastChild; }
	const BiXmlNodeImpl* LastChild( const char * value ) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	BiXmlNodeImpl* LastChild( const char * value );	

    #ifdef BIXML_USE_STL
	const BiXmlNode* FirstChild( const std::string& _value ) const	{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	BiXmlNode* FirstChild( const std::string& _value )				{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
	const BiXmlNode* LastChild( const std::string& _value ) const	{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	BiXmlNode* LastChild( const std::string& _value )				{	return LastChild (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	const BiXmlNodeImpl* IterateChildren( const BiXmlNodeImpl* previous ) const;
	BiXmlNodeImpl* IterateChildren( BiXmlNodeImpl* previous );

	const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const;
	BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous );
	/// This flavor of IterateChildren searches for children with a particular 'value'
	const BiXmlNodeImpl* _IterateChildren( const char * value, const BiXmlNodeImpl* previous ) const;
	BiXmlNodeImpl* _IterateChildren( const char * value, BiXmlNodeImpl* previous );

    #ifdef BIXML_USE_STL
	const BiXmlNode* IterateChildren( const std::string& _value, const BiXmlNode* previous ) const	{	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	BiXmlNode* IterateChildren( const std::string& _value, BiXmlNode* previous ) {	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
	#endif

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	BiXmlNode* InsertEndChild( const BiXmlNode& addThis );
	BiXmlNode* InsertEndChild( const char* name);//added by wangheng 2011.11.24

	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	BiXmlNode* LinkEndChild( BiXmlNode* addThis );
	
	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	BiXmlNodeImpl* InsertBeforeChild( BiXmlNodeImpl* beforeThis, const BiXmlNodeImpl& addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	BiXmlNodeImpl* InsertAfterChild(  BiXmlNodeImpl* afterThis, const BiXmlNodeImpl& addThis );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	BiXmlNodeImpl* ReplaceChild( BiXmlNodeImpl* replaceThis, const BiXmlNodeImpl& withThis );

	/// Delete a child of this node.
	bool _RemoveChild( BiXmlNodeImpl* removeThis );

	/// Navigate to a sibling node.
	const BiXmlNodeImpl* PreviousSibling() const			{ return prev; }
	BiXmlNodeImpl* PreviousSibling()						{ return prev; }

	/// Navigate to a sibling node.
	const BiXmlNodeImpl* PreviousSibling( const char * ) const;
	BiXmlNodeImpl* PreviousSibling( const char * );

    #ifdef BIXML_USE_STL
	const BiXmlNode* PreviousSibling( const std::string& _value ) const	{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	BiXmlNode* PreviousSibling( const std::string& _value ) 			{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
	const BiXmlNode* NextSibling( const std::string& _value) const		{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	BiXmlNode* NextSibling( const std::string& _value) 					{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Navigate to a sibling node.
	const BiXmlNodeImpl* _NextSibling() const				{ return next; }
	BiXmlNodeImpl* _NextSibling()							{ return next; }
	const BiXmlNode* NextSibling() const				{ return next; }
	BiXmlNode* NextSibling()							{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	const BiXmlNode* NextSibling( const char * value) const;
	
	BiXmlNode* NextSibling( const char * value);

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const BiXmlElement* NextSiblingElement() const;
	
	BiXmlElement* NextSiblingElement();
	

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const BiXmlElement* NextSiblingElement( const char * value) const;
	
	BiXmlElement* NextSiblingElement( const char * value);	

    #ifdef BIXML_USE_STL
	const BiXmlElement* NextSiblingElement( const std::string& _value) const	{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	BiXmlElement* NextSiblingElement( const std::string& _value)				{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/// Convenience function to get through elements.
	const BiXmlElement* FirstChildElement()	const;
	
	BiXmlElement* FirstChildElement();
	
	/// Convenience function to get through elements.
	const BiXmlElement* FirstChildElement( const char * value ) const;
	
	BiXmlElement* FirstChildElement( const char * value );
	bool RemoveChild( BiXmlNode* removeThis );
	
    #ifdef BIXML_USE_STL
	const BiXmlElement* FirstChildElement( const std::string& _value ) const	{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	BiXmlElement* FirstChildElement( const std::string& _value )				{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
	#endif

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: DOCUMENT, ELEMENT, COMMENT,
								UNKNOWN, TEXT, and DECLARATION.
	*/
	int Type() const	{ return type; }
	virtual DocType GetXmlType() {return DT_XML;};

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	const BiXmlDocument* GetDocument() const;
	
	BiXmlDocument* GetDocument();

	/// Returns true if this node has no children.
	bool NoChildren() const						{ return !firstChild; }

	virtual const BiXmlDocumentImpl*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual BiXmlDocumentImpl*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlElement*           ToElement()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlUnknown*           ToUnknown()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlText*	            ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** Create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual BiXmlNode* Clone() const
	{
		return _Clone();
	}

	///--------BiXml内部使用的接口
	virtual BiXmlNodeImpl* _Clone()const = 0;

	const BiXmlNodeImpl*	_FirstChild()const				{ return firstChild; }		///< The first child of this node. Will be null if there are no children.
	BiXmlNodeImpl*			_FirstChild()					{ return firstChild; }
	const BiXmlNodeImpl*	_FirstChild( const char * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	BiXmlNodeImpl*			_FirstChild( const char * value );	

	const BiXmlElementImpl* _FirstChildElement()	const;
	BiXmlElementImpl*		_FirstChildElement();

	/// Convenience function to get through elements.
	const BiXmlElementImpl* _FirstChildElement( const char * value ) const;
	BiXmlElementImpl*		_FirstChildElement( const char * value );

	const BiXmlDocumentImpl*	_GetDocument() const;
	BiXmlDocumentImpl*			_GetDocument();

	BiXmlNodeImpl* _InsertEndChild( const BiXmlNodeImpl& _addThis );
	BiXmlNodeImpl* _LinkEndChild(BiXmlNodeImpl* node);
	
	const BiXmlNodeImpl*	_NextSibling( const char * ) const;
	BiXmlNodeImpl*	_NextSibling( const char * );

	const BiXmlElementImpl* _NextSiblingElement() const;
	BiXmlElementImpl*		_NextSiblingElement();

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const BiXmlElementImpl*		_NextSiblingElement( const char * ) const;
	BiXmlElementImpl*			_NextSiblingElement( const char * );

	const BiXmlNodeImpl*		_Parent() const{ return parent; }
	
	virtual const BiXmlElementImpl*	_ToElement()const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlElementImpl*		_ToElement(){ return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const BiXmlTextImpl*		_ToText()const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual BiXmlTextImpl*				_ToText(){ return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	

	
#ifdef BIXML_USE_STL
	// The real work of the input operator.
	virtual void StreamIn( BIXML_ISTREAM* in, BIXML_STRING* tag ) = 0;
#endif

protected:
	BiXmlNodeImpl( NodeType _type );

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void CopyTo( BiXmlNodeImpl* target ) const;

	
	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	BiXmlNodeImpl* Identify( const char* start, BiXmlEncoding encoding );

	BiXmlNodeImpl*		parent;
	NodeType		type;

	BiXmlNodeImpl*		firstChild;
	BiXmlNodeImpl*		lastChild;

	BIXML_STRING	value;

	BiXmlNodeImpl*		prev;
	BiXmlNodeImpl*		next;

private:
	BiXmlNodeImpl( const BiXmlNodeImpl& );				// not implemented.
	void operator=( const BiXmlNodeImpl& base );	// not allowed.
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not BiXmlNodeImpls, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class BiXmlAttribute : public BiXmlBase
{
	friend class BiXmlAttributeSet;
	friend class BiXmlFactory;

public:
	virtual void Release();
	/// Construct an empty attribute.
	BiXmlAttribute() : BiXmlBase()
	{
		document = 0;
		prev = next = 0;
	}

	#ifdef BIXML_USE_STL
	/// std::string constructor.
	BiXmlAttribute( const std::string& _name, const std::string& _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}
	#endif

	/// Construct an attribute with a name and value.
	BiXmlAttribute( const char * _name, const char * _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}
	const char*		Name()  const		{ return name.c_str (); }		///< Return the name of this attribute.
	const char*		Value() const		{ return value.c_str (); }		///< Return the value of this attribute.
	int				IntValue() const;									///< Return the value of this attribute, converted to an integer.
	double			DoubleValue() const;								///< Return the value of this attribute, converted to a double.

	// Get the tinyxml string representation
	const BIXML_STRING& NameTStr() const { return name; }

	/** QueryIntValue examines the value string. It is an alternative to the
		IntValue() method with richer error checking.
		If the value is an integer, it is stored in 'value' and 
		the call returns BIXML_SUCCESS. If it is not
		an integer, it returns BIXML_WRONG_TYPE.

		A specialized but useful call. Note that for success it returns 0,
		which is the opposite of almost all other TinyXml calls.
	*/
	int QueryIntValue( int* _value ) const;
	/// QueryDoubleValue examines the value string. See QueryIntValue().
	int QueryDoubleValue( double* _value ) const;

	void SetName( const char* _name )	{ name = _name; }				///< Set the name of this attribute.
	void SetValue( const char* _value )	{ value = _value; }				///< Set the value.

	void SetIntValue( int _value );										///< Set the value from an integer.
	void SetDoubleValue( double _value );								///< Set the value from a double.

    #ifdef BIXML_USE_STL
	/// STL std::string form.
	void SetName( const std::string& _name )	{ name = _name; }	
	/// STL std::string form.	
	void SetValue( const std::string& _value )	{ value = _value; }
	#endif

	/// Get the next sibling attribute in the DOM. Returns null at end.
	const BiXmlAttribute* Next() const;
	BiXmlAttribute* Next();
	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	const BiXmlAttribute* Previous() const;
	BiXmlAttribute* Previous();

	bool operator==( const BiXmlAttribute& rhs ) const { return rhs.name == name; }
	bool operator<( const BiXmlAttribute& rhs )	 const { return name < rhs.name; }
	bool operator>( const BiXmlAttribute& rhs )  const { return name > rhs.name; }

	/*	Attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	// Prints this Attribute to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual void StreamOut( BIXML_OSTREAM * out ) const;
	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument( BiXmlDocumentImpl* doc )	{ document = doc; }

private:
	BiXmlAttribute( const BiXmlAttribute& );				// not implemented.
	void operator=( const BiXmlAttribute& base );	// not allowed.

	BiXmlDocumentImpl*	document;	// A pointer back to a document, for error reporting.
	BIXML_STRING name;
	BIXML_STRING value;
	BiXmlAttribute*	prev;
	BiXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class BiXmlAttributeSet
{
	friend class BiXmlElementImpl;
private:
	BiXmlAttributeSet();
	~BiXmlAttributeSet();
public:
	void Add( BiXmlAttribute* attribute );
	void Remove( BiXmlAttribute* attribute );

	const BiXmlAttribute* First()	const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	BiXmlAttribute* First()					{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	const BiXmlAttribute* Last() const		{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
	BiXmlAttribute* Last()					{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }

	const BiXmlAttribute*	Find( const BIXML_STRING& name ) const;
	BiXmlAttribute*	Find( const BIXML_STRING& name );

private:
	//*ME:	Because of hidden/disabled copy-construktor in BiXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	BiXmlAttributeSet( const BiXmlAttributeSet& );	// not allowed
	void operator=( const BiXmlAttributeSet& );	// not allowed (as BiXmlAttribute)

	BiXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class BiXmlElementImpl : public BiXmlNodeImpl, public BiXmlElement
{
	friend class BiXmlFactory;
public:
	BIXML_PUBLIC_INHERIT
	virtual BiXmlNode* Clone() const{ return _Clone(); }
	virtual const BiXmlText*	ToText()const{	return BiXmlNodeImpl::ToText(); }
	virtual BiXmlText*	ToText(){ return BiXmlNodeImpl::ToText(); }
	virtual const BiXmlElement*     ToElement()const{ return _ToElement(); }
	virtual BiXmlElement*           ToElement(){ return _ToElement(); }
public:
	/// Construct an element.
	BiXmlElementImpl (const char * in_value);

	#ifdef BIXML_USE_STL
	/// std::string constructor.
	BiXmlElementImpl( const std::string& _value );
	#endif

	BiXmlElementImpl( const BiXmlElementImpl& );

	virtual ~BiXmlElementImpl();
public:

	virtual void Release();
	void operator=( const BiXmlElementImpl& base );
	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	const char* Attribute( const char* name ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	const char* Attribute( const char* name, int* i ) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	const char* Attribute( const char* name, double* d ) const;

	/** QueryIntAttribute examines the attribute - it is an alternative to the
		Attribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and 
		the call returns BIXML_SUCCESS. If it is not
		an integer, it returns BIXML_WRONG_TYPE. If the attribute
		does not exist, then BIXML_NO_ATTRIBUTE is returned.
	*/	
	int QueryIntAttribute( const char* name, int* _value ) const;
	/// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
	int QueryDoubleAttribute( const char* name, double* _value ) const;
	/// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
	int QueryFloatAttribute( const char* name, float* _value ) const {
		double d;
		int result = QueryDoubleAttribute( name, &d );
		if ( result == BIXML_SUCCESS ) {
			*_value = (float)d;
		}
		return result;
	}

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char* name, const char * _value );

    #ifdef BIXML_USE_STL
	const char* Attribute( const std::string& name ) const				{ return Attribute( name.c_str() ); }
	const char* Attribute( const std::string& name, int* i ) const		{ return Attribute( name.c_str(), i ); }
	const char* Attribute( const std::string& name, double* d ) const	{ return Attribute( name.c_str(), d ); }
	int QueryIntAttribute( const std::string& name, int* _value ) const	{ return QueryIntAttribute( name.c_str(), _value ); }
	int QueryDoubleAttribute( const std::string& name, double* _value ) const { return QueryDoubleAttribute( name.c_str(), _value ); }

	/// STL std::string form.
	void SetAttribute( const std::string& name, const std::string& _value );
	///< STL std::string form.
	void SetAttribute( const std::string& name, int _value );
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const char * name, int value );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetDoubleAttribute( const char * name, double value );

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const char * name );
    #ifdef BIXML_USE_STL
	void RemoveAttribute( const std::string& name )	{	RemoveAttribute (name.c_str ());	}	///< STL std::string form.
	#endif

	const BiXmlAttribute* FirstAttribute() const	{ return attributeSet.First(); }		///< Access the first attribute in this element.
	BiXmlAttribute* FirstAttribute() 				{ return attributeSet.First(); }
	const BiXmlAttribute* LastAttribute()	const 	{ return attributeSet.Last(); }		///< Access the last attribute in this element.
	BiXmlAttribute* LastAttribute()					{ return attributeSet.Last(); }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the BiXmlTextImpl child
		and accessing it directly.
	
		If the first child of 'this' is a BiXmlTextImpl, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the 
				 similarly named BiXmlHandle::Text() and BiXmlNodeImpl::ToText() which are 
				 safe type casts on the referenced node.
	*/
	const char* GetText() const;
	bool SetText(const char* strText);
	bool AddText(const char* strText);

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual BiXmlNodeImpl* _Clone() const;
	// Print the Element to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	virtual const BiXmlElementImpl*     _ToElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlElementImpl*           _ToElement()	          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	
protected:

	void CopyTo( BiXmlElementImpl* target ) const;
	void ClearThis();	// like clear, but initializes 'this' object as well

	public:
	// Used to be public [internal use]
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif

		virtual void StreamOut( BIXML_OSTREAM * out ) const;
		protected:

	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* ReadValue( const char* in, BiXmlParsingData* prevData, BiXmlEncoding encoding );

private:

	BiXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class BiXmlComment : public BiXmlNodeImpl
{
public:
	friend class BiXmlFactory;
	/// Constructs an empty comment.
	BiXmlComment() : BiXmlNodeImpl( BiXmlNodeImpl::COMMENT ) {}
	BiXmlComment( const BiXmlComment& );
	

	virtual ~BiXmlComment()	{}
	virtual void Release();
	void operator=( const BiXmlComment& base );
	/// Returns a copy of this Comment.
	virtual BiXmlNodeImpl* _Clone() const;
	/// Write this Comment to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	virtual const BiXmlComment*  ToComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlComment*  ToComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( BiXmlComment* target ) const;

	// used to be public
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif
public:
	virtual void StreamOut( BIXML_OSTREAM * out ) const;

};


/** XML text. A text node can have 2 ways to output the next. "normal" output 
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with 
	SetCDATA() and query it with CDATA().
*/
class BiXmlTextImpl : public BiXmlNodeImpl, public BiXmlText
{
	friend class BiXmlElementImpl;
	friend class BiXmlFactory;
public:
	BIXML_PUBLIC_INHERIT
	///----------接口实现-----
	virtual BiXmlNode* Clone() const{return _Clone();}
	virtual const BiXmlElement*     ToElement()const{return BiXmlNodeImpl::ToElement();	}
	virtual BiXmlElement*           ToElement(){return BiXmlNodeImpl::ToElement();}
	virtual BiXmlText* ToText(){return _ToText();};
	virtual const BiXmlText* ToText() const {return _ToText();};
	virtual void Release();

	/** Constructor for text element. By default, it is treated as 
		normal, encoded text. If you want it be output as a CDATA text
		element, set the parameter _cdata to 'true'
	*/
	BiXmlTextImpl (const char * initValue ) : BiXmlNodeImpl (BiXmlNodeImpl::TEXT)
	{
		SetValue( initValue );
		cdata = false;
	}
	virtual ~BiXmlTextImpl() {}

	#ifdef BIXML_USE_STL
	/// Constructor.
	BiXmlTextImpl( const std::string& initValue ) : BiXmlNodeImpl (BiXmlNodeImpl::TEXT)
	{
		SetValue( initValue.c_str() );
		cdata = false;
	}
	#endif

	BiXmlTextImpl( const BiXmlTextImpl& copy ) : BiXmlNodeImpl( BiXmlNodeImpl::TEXT )	{ copy.CopyTo( this ); }
public:
	void operator=( const BiXmlTextImpl& base )							 	{ base.CopyTo( this ); }

	/// Write this text object to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	/// Queries whether this represents text using a CDATA section.
	bool CDATA()					{ return cdata; }
	/// Turns on or off a CDATA representation of text.
	void SetCDATA( bool _cdata )	{ cdata = _cdata; }

	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	virtual const BiXmlTextImpl* _ToText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlTextImpl*       _ToText()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected :
	///  [internal use] Creates a new Element and returns it.
	virtual BiXmlNodeImpl* _Clone() const;
	void CopyTo( BiXmlTextImpl* target ) const;

public:
	virtual void StreamOut ( BIXML_OSTREAM * out ) const;
protected:
	bool Blank() const;	// returns true if all white space and new lines
public:
	// [internal use]
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif

private:
	bool cdata;			// true if this should be input and output as a CDATA style text element
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class BiXmlDeclaration : public BiXmlNodeImpl
{
public:
	friend class BiXmlFactory;
//private:
	/// Construct an empty declaration.
	BiXmlDeclaration()   : BiXmlNodeImpl( BiXmlNodeImpl::DECLARATION ) {}

#ifdef BIXML_USE_STL
	/// Constructor.
	BiXmlDeclaration(	const std::string& _version,
						const std::string& _encoding,
						const std::string& _standalone );
#endif

	/// Construct.
	BiXmlDeclaration(	const char* _version,
						const char* _encoding,
						const char* _standalone );

	BiXmlDeclaration( const BiXmlDeclaration& copy );
	

	virtual ~BiXmlDeclaration()	{}
	void operator=( const BiXmlDeclaration& copy );
	virtual void Release();
	/// Version. Will return an empty string if none was found.
	const char *Version() const			{ return version.c_str (); }
	/// Encoding. Will return an empty string if none was found.
	const char *Encoding() const		{ return encoding.c_str (); }
	/// Is this a standalone document?
	const char *Standalone() const		{ return standalone.c_str (); }

	/// Creates a copy of this Declaration and returns it.
	virtual BiXmlNodeImpl* _Clone() const;
	/// Print this declaration to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	virtual const BiXmlDeclaration* ToDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlDeclaration*       ToDeclaration()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( BiXmlDeclaration* target ) const;
public:
	// used to be public
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif
		virtual void StreamOut ( BIXML_OSTREAM * out) const;

private:

	BIXML_STRING version;
	BIXML_STRING encoding;
	BIXML_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into BiXmlUnknowns.
*/
class BiXmlUnknown : public BiXmlNodeImpl
{
public:
	friend class BiXmlFactory;
	BiXmlUnknown() : BiXmlNodeImpl( BiXmlNodeImpl::UNKNOWN )	{}
	virtual ~BiXmlUnknown() {}

	BiXmlUnknown( const BiXmlUnknown& copy ) : BiXmlNodeImpl( BiXmlNodeImpl::UNKNOWN )		{ copy.CopyTo( this ); }
	void operator=( const BiXmlUnknown& copy )										{ copy.CopyTo( this ); }
	virtual void Release();
	/// Creates a copy of this Unknown and returns it.
	virtual BiXmlNodeImpl* _Clone() const;
	/// Print this Unknown to a FILE stream.
	virtual void Print( FILE* cfile, int depth ) const;

	virtual const char* _Parse( const char* p, BiXmlParsingData* data, BiXmlEncoding encoding );

	virtual const BiXmlUnknown*     ToUnknown()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlUnknown*           ToUnknown()	    { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

protected:
	void CopyTo( BiXmlUnknown* target ) const;
public:
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif
	virtual void StreamOut ( BIXML_OSTREAM * out ) const;

private:

};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class BiXmlDocumentImpl : public BiXmlNodeImpl, public BiXmlDocument
{
public:
	BIXML_PUBLIC_INHERIT
	virtual BiXmlNode* Clone() const{return _Clone();}

	virtual const BiXmlElement*     ToElement()const{return BiXmlNodeImpl::ToElement();}
	virtual BiXmlElement*           ToElement(){return BiXmlNodeImpl::ToElement();}
	virtual const BiXmlText*        ToText()const{	return BiXmlNodeImpl::ToText();}
	virtual BiXmlText*        ToText(){	return BiXmlNodeImpl::ToText();}

	///---------接口添加
	virtual bool InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone);
	virtual const char* ParseXmlStr(const char* strContent);
	virtual const char* Parse(const char* strContent);
	virtual bool Error()const;
	virtual void Release();

//public:
	/// Create an empty document, that has no name.
	BiXmlDocumentImpl();
	/// Create a document with a name. The name of the document is also the filename of the xml.
	BiXmlDocumentImpl( const char * documentName );

	#ifdef BIXML_USE_STL
	/// Constructor.
	BiXmlDocumentImpl( const std::string& documentName );
	#endif

	BiXmlDocumentImpl( const BiXmlDocumentImpl& copy );


	virtual ~BiXmlDocumentImpl() {}

public:
	void operator=( const BiXmlDocumentImpl& copy );
	friend class BiXmlFactory;
	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile( BiXmlEncoding encoding = BIXML_DEFAULT_ENCODING );
	/// Save a file using the current document value. Returns true if successful.
	//bool SaveFile() const;
	bool SaveFile();
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const char * filename, BiXmlEncoding encoding = BIXML_DEFAULT_ENCODING );
	/// Save a file using the given filename. Returns true if successful.
	//bool SaveFile( const char * filename ) const;
	bool SaveFile( const char * filename );
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool LoadFile( FILE*, BiXmlEncoding encoding = BIXML_DEFAULT_ENCODING );
	/// Save a file using the given FILE*. Returns true if successful.
	bool SaveFile( FILE* ) const;

	#ifdef BIXML_USE_STL
	bool LoadFile( const std::string& filename, BiXmlEncoding encoding = BIXML_DEFAULT_ENCODING )			///< STL std::string version.
	{
		StringToBuffer f( filename );
		return ( f.buffer && LoadFile( f.buffer, encoding ));
	}
	bool SaveFile( const std::string& filename ) const		///< STL std::string version.
	{
		StringToBuffer f( filename );
		return ( f.buffer && SaveFile( f.buffer ));
	}
	#endif

	/** Parse the given null terminated block of xml data. Passing in an encoding to this
		method (either BIXML_ENCODING_LEGACY or BIXML_ENCODING_UTF8 will force TinyXml
		to use that encoding, regardless of what TinyXml might otherwise try to detect.
	*/
	virtual const char* _Parse( const char* p, BiXmlParsingData* data = 0, BiXmlEncoding encoding = BIXML_DEFAULT_ENCODING );

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	const BiXmlElement* RootElement() const		{ return FirstChildElement(); }
	BiXmlElement* RootElement()					{ return FirstChildElement(); }

	/** If an error occurs, Error will be set to true. Also,
		- The ErrorId() will contain the integer identifier of the error (not generally useful)
		- The ErrorDesc() method will return the name of the error. (very useful)
		- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	*/	
	bool _Error() const						{ return error; }

	/// Contains a textual (english) description of the error if one occurs.
	const char * ErrorDesc() const	{ return errorDesc.c_str (); }

	/** Generally, you probably want the error string ( ErrorDesc() ). But if you
		prefer the ErrorId, this function will fetch it.
	*/
	int ErrorId()	const				{ return errorId; }

	/** Returns the location (if known) of the error. The first column is column 1, 
		and the first row is row 1. A value of 0 means the row and column wasn't applicable
		(memory errors, for example, have no row/column) or the parser lost the error. (An
		error in the error reporting, in that case.)

		@sa SetTabSize, Row, Column
	*/
	int ErrorRow()	{ return errorLocation.row+1; }
	int ErrorCol()	{ return errorLocation.col+1; }	///< The column where the error occured. See ErrorRow()

	/** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		BiXmlDocumentImpl doc;
		doc.SetTabSize( 8 );
		doc.Load( "myfile.xml" );
		@endverbatim

		@sa Row, Column
	*/
	void SetTabSize( int _tabsize )		{ tabsize = _tabsize; }

	int TabSize() const	{ return tabsize; }

	/** If you have handled the error, it can be reset with this call. The error
		state is automatically cleared if you Parse a new XML block.
	*/
	void ClearError()						{	error = false; 
												errorId = 0; 
												errorDesc = ""; 
												errorLocation.row = errorLocation.col = 0; 
												//errorLocation.last = 0; 
											}

	/** Dump the document to standard out. */
	void Print() const						{ Print( stdout, 0 ); }

	/// Print this Document to a FILE stream.
	virtual void Print( FILE* cfile, int depth = 0 ) const;
	// [internal use]
	void SetError( int err, const char* errorLocation, BiXmlParsingData* prevData, BiXmlEncoding encoding );

	virtual const BiXmlDocumentImpl*    ToDocument()    const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual BiXmlDocumentImpl*          ToDocument()          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
protected :
public:
	virtual void StreamOut ( BIXML_OSTREAM * out) const;
protected:
	bool LoadFileFromPack(const char* filename,BiXmlEncoding encoding );
	// [internal use]
	virtual BiXmlNodeImpl* _Clone() const;
	#ifdef BIXML_USE_STL
	    virtual void StreamIn( BIXML_ISTREAM * in, BIXML_STRING * tag );
	#endif

private:
	void CopyTo( BiXmlDocumentImpl* target ) const;

	bool error;
	int  errorId;
	BIXML_STRING errorDesc;
	int tabsize;
	BiXmlCursor errorLocation;
	bool useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
	A BiXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that BiXmlHandle is not part of the TinyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	BiXmlElementImpl* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		BiXmlElementImpl* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			BiXmlElementImpl* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				BiXmlElementImpl* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. BiXmlHandle addresses the verbosity
	of such code. A BiXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	BiXmlHandle docHandle( &document );
	BiXmlElementImpl* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).Element();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	BiXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		BiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).Element();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	BiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).Element();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class BiXmlHandle
{
//public:
private:
	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	BiXmlHandle( BiXmlNodeImpl* _node )					{ this->node = _node; }
	/// Copy constructor
	BiXmlHandle( const BiXmlHandle& ref )			{ this->node = ref.node; }
public:
	BiXmlHandle operator=( const BiXmlHandle& ref ) { this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	BiXmlHandle FirstChild() const;
	/// Return a handle to the first child node with the given name.
	BiXmlHandle FirstChild( const char * value ) const;
	/// Return a handle to the first child element.
	BiXmlHandle FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	BiXmlHandle FirstChildElement( const char * value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	BiXmlHandle Child( const char* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	BiXmlHandle Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only BiXmlElements
		are indexed: other types are not counted.
	*/
	BiXmlHandle ChildElement( const char* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only BiXmlElements
		are indexed: other types are not counted.
	*/
	BiXmlHandle ChildElement( int index ) const;

	#ifdef BIXML_USE_STL
	BiXmlHandle FirstChild( const std::string& _value ) const				{ return FirstChild( _value.c_str() ); }
	BiXmlHandle FirstChildElement( const std::string& _value ) const		{ return FirstChildElement( _value.c_str() ); }

	BiXmlHandle Child( const std::string& _value, int index ) const			{ return Child( _value.c_str(), index ); }
	BiXmlHandle ChildElement( const std::string& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
	#endif

	/// Return the handle as a BiXmlNodeImpl. This may return null.
	BiXmlNodeImpl* Node() const			{ return node; } 
	/// Return the handle as a BiXmlElementImpl. This may return null.
	BiXmlElement* Element() const	{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	/// Return the handle as a BiXmlText. This may return null.
	BiXmlTextImpl* Text() const			{ return ( ( node && node->_ToText() ) ? node->_ToText() : 0 ); }
	/// Return the handle as a BiXmlUnknown. This may return null;
	BiXmlUnknown* Unknown() const			{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

private:
	BiXmlNodeImpl* node;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif

