#ifndef __BINXML__H_kk0skdkf__
#define __BINXML__H_kk0skdkf__

#include "../MemAllocator.h"
#include <map>
#include <list>
#include <string>
#include <ostream>
#include <vector>
#include "../IXMLNode.h"
#include <cassert>

inline void __no_print(...){}

#ifdef _DEBUG
#define reader_print printf
#else
#define reader_print __no_print
#endif

enum BXML_TYPE
{
	BT_INVALID			= 0,
	BT_DOCUMENT			= 'D',
	BT_END_DOCUMENT		= 'Z',
	BT_ELEMENT			= 'e',
	BT_END_ELEMENT		= 'z',
	BT_ATTRIBUTE		= 'a',

	//values
	BT_INDEXED_STRING	= 'x',
	BT_BYTE				= 'b',
	BT_WORD				= 'w',
	BT_DWORD			= 'd',
	BT_FLOAT			= 'f',
	BT_DIGIT_BEGIN		= '0',
	BT_DIGIT_END		= '9',
	//end values
};

struct xml_io_exception
{
	int pos;
};

class string_dictionary
{
	typedef std::list<std::string> StrList;
	StrList m_strings;

	typedef const char* str_type;

	str_type* m_str_map;
	int m_index_counter;
	int m_max_size;
public:
	enum{MAX_STRING = 0xffff};

	string_dictionary()
	{
		m_index_counter = 0;
		m_max_size = 0;
		m_str_map = NULL;

		/*m_str_map = new str_type[MAX_STRING];
		memset(m_str_map, 0, sizeof(str_type)*MAX_STRING);*/
	}
	void init(int size)
	{
		if (size < 1)
		{
			assert(false);
		}

		if (m_str_map)
			delete []m_str_map;

		m_index_counter = 0;
		m_str_map = new str_type[size];
		memset(m_str_map, 0, sizeof(str_type)*size);
		m_max_size = size;
	}
	void clear()
	{
		if (m_str_map)
		{
			delete []m_str_map;
			m_str_map = NULL;
			m_max_size = 0;
		}
	}
	~string_dictionary()
	{
		/*if (m_str_map)
			delete []m_str_map;*/
		clear();
	}
public:
	const char* find_string(int index)
	{
		//if(index<MAX_STRING)
		if(index < m_max_size)
		{
			return m_str_map[index];
		}
		else
		{
			return 0;
		}
	}
	void add_pair(int index, const char* str)
	{
		const char* old = find_string(index);
		if(old)
		{
			//printf("already exists: %d,old=%s, new=%s", index, old, str);
			reader_print("already exists: %d,old=%s, new=%s", index, old, str);
		}
		m_str_map[index] = str;
	}
	int add_string(const char* str)
	{
		std::string s = str;

		for(int i=0;i<m_index_counter;++i)
		{
			if(m_str_map[i] && s == m_str_map[i])
				return i;
		}

		//not found, so make a new one
		//if(m_index_counter < MAX_STRING)
		if (m_index_counter < m_max_size)
		{
			int index = m_index_counter++;
			add_pair(index, str);
			return index;
		}
		else
		{
			//printf("maximum strings exceeded\n");
			reader_print("maximum strings exceeded\n");
			return -1;
		}
	}

	int save_int(char* buffer, int value)
	{
		memcpy(buffer, &value, sizeof(value));
		return sizeof(value);
	}
	int save_string(char* buffer, const char* str)
	{
		int len = strlen(str)+1;
		int pos=save_int(buffer, len);
		memcpy(buffer+pos, str, len);
		pos+=len;
		return pos;
	}
	int load_int(const char* buffer, int& value)
	{
		memcpy(&value, buffer, sizeof(value));
		return sizeof(value);
	}
	int load_string(const char* buffer, const char*& str)
	{
		int len;
		int pos=load_int(buffer, len);
		str = buffer+pos;
		pos+=len;
		return pos;
	}

	int save(char* buffer, int max_len)
	{
		int pos = save_int(buffer, m_index_counter);
		for(int i=0;i<m_index_counter;++i)
		{
			int index = i;
			const char* str = m_str_map[i];
			pos+=save_int(buffer+pos, index);
			pos+=save_string(buffer+pos, str);
			if(pos>max_len)
			{
				//printf("save buffer exceeded"); //不太准，不过问题肯定可以探测到
				reader_print("save buffer exceeded"); 
				__asm int 3;
				break;
			}
		}
		return pos;
	}

	void load(const char* buffer, int max_len)
	{
		int count=0;
		int pos = load_int(buffer, count);
		init(count);
		for(int i=0;i<count;++i)
		{
			int index;
			const char* str;
			pos+=load_int(buffer+pos, index);
			pos+=load_string(buffer+pos, str);

			if(pos>max_len)
			{
				//printf("read buffer exceeded"); //不太准，不过问题肯定可以探测到
				reader_print("read buffer exceeded"); 
				break;
			}
			this->add_pair(index, str);
		}
	}
};

class xml_buffer
{
	const char* m_buff;
	int m_length;
	int m_read_pos;
public:
	xml_buffer(const char* b, int l)
	{
		m_read_pos = 0;
		m_length = l;
		m_buff = b;
	}
	~xml_buffer()
	{
	}
protected:
	void read_next(void* to, int len)
	{
		if(len > m_length-m_read_pos)
		{
			throw_exception();
		}
		memcpy(to, m_buff+m_read_pos, len);
		m_read_pos+=len;
	}
	void write_next(const void* from, int len)
	{
	}
public:
	void throw_exception()
	{
		xml_io_exception e;
		e.pos = this->position();
		throw e;
	}
	int position()
	{
		return m_read_pos;
	}
	void read(char& data)
	{
		read_next(&data, sizeof(data));
	}
	void read(short& data)
	{
		read_next(&data, sizeof(data));
	}
	void read(int& data)
	{
		read_next(&data, sizeof(data));
	}
	void read(float& data)
	{
		read_next(&data, sizeof(data));
	}
	void read(BXML_TYPE& data)
	{
		data=BT_INVALID;
		read_next(&data, sizeof(char));
	}
};

struct BinXML_Header
{
	short magic_word;
	char version;
	char headlen;
	int dict_len;
	int content_len;
	int flags;
	int reserved;
};

class BiBxmlAttributeImpl;
class BiBxmlDocumentImpl;
class BiBxmlElementImpl;
//属性
BiBxmlAttributeImpl*		BiBxmlCreateAttribute();
//文档
BiBxmlDocumentImpl*	BiBxmlCreateDocument();
void				BiBxmlReleaseDocument(BiBxmlDocumentImpl*	doc);
//元素
BiBxmlElementImpl*	BiBxmlCreateElement(const char* _value);
void				BiBxmlReleaseElement(BiBxmlElementImpl* elem);

class BiBxmlFactory
{
public:
	static BiBxmlAttributeImpl*		CreateAttribute();
	static BiBxmlDocumentImpl*	CreateDocument();
	static BiBxmlElementImpl*	CreateElement(const char* _value);

	static void					ReleaseAttribute(BiBxmlAttributeImpl* attr);
	static void					ReleaseDocument(BiBxmlDocumentImpl* doc);
	static void					ReleaseElement(BiBxmlElementImpl* elem);

private:
	static ClassPool<BiBxmlAttributeImpl, CBiboLock>		m_AttributePool;
	static ClassPool<BiBxmlDocumentImpl, CBiboLock>	m_DocumentPool;
	static ClassPool<BiBxmlElementImpl, CBiboLock>	m_ElementPool;
};

#define BIBXML_PUBLIC_INHERIT \
	virtual void Clear(){return BiBxmlNodeImpl::Clear();}\
	virtual const BiXmlNode* FirstChild()const{	return BiBxmlNodeImpl::FirstChild();}\
	virtual BiXmlNode* FirstChild(){	return BiBxmlNodeImpl::FirstChild();}\
	virtual const BiXmlNode* FirstChild( const char * value ) const{return BiBxmlNodeImpl::FirstChild(value);}\
	virtual BiXmlNode* FirstChild( const char * value ) {return BiBxmlNodeImpl::FirstChild(value);}\
	virtual const BiXmlElement* FirstChildElement()	const{return BiBxmlNodeImpl::FirstChildElement();}\
	virtual BiXmlElement* FirstChildElement(){return BiBxmlNodeImpl::FirstChildElement();}\
	virtual const BiXmlElement* FirstChildElement( const char * value ) const{return BiBxmlNodeImpl::FirstChildElement(value);}\
	virtual BiXmlElement* FirstChildElement( const char * value ){return BiBxmlNodeImpl::FirstChildElement(value);}\
	virtual const BiXmlNode* NextSibling() const{return BiBxmlNodeImpl::NextSibling();}\
	virtual BiXmlNode* NextSibling(){return BiBxmlNodeImpl::NextSibling();}\
	virtual const BiXmlNode* NextSibling( const char * _value) const{return BiBxmlNodeImpl::NextSibling(_value);}\
	virtual BiXmlNode* NextSibling( const char * _value){return BiBxmlNodeImpl::NextSibling(_value);}\
	virtual const BiXmlElement* NextSiblingElement() const{	return BiBxmlNodeImpl::NextSiblingElement();}\
	virtual BiXmlElement* NextSiblingElement(){	return BiBxmlNodeImpl::NextSiblingElement();}\
	virtual const BiXmlElement* NextSiblingElement( const char * _value) const{	return BiBxmlNodeImpl::NextSiblingElement(_value);}\
	virtual BiXmlElement* NextSiblingElement( const char * _value){	return BiBxmlNodeImpl::NextSiblingElement(_value);}\
	virtual const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const{return BiBxmlNodeImpl::IterateChildren(value, previous);}\
	virtual BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous ){return BiBxmlNodeImpl::IterateChildren(value, previous);}\
	virtual const BiXmlDocument* GetDocument() const{return BiBxmlNodeImpl::GetDocument();}\
	virtual BiXmlDocument* GetDocument(){return BiBxmlNodeImpl::GetDocument();}\
	virtual const BiXmlNode* Parent() const{return BiBxmlNodeImpl::Parent();}\
	virtual BiXmlNode* InsertEndChild( const BiXmlNode& addThis ){return BiBxmlNodeImpl::InsertEndChild(addThis);}\
	virtual BiXmlNode* InsertEndChild( const char* strName ){return BiBxmlNodeImpl::InsertEndChild(strName);}\
	virtual BiXmlNode* LinkEndChild( BiXmlNode* addThis ){return BiBxmlNodeImpl::LinkEndChild(addThis);}\
	virtual bool RemoveChild( BiXmlNode* removeThis ){return BiBxmlNodeImpl::RemoveChild(removeThis);}\
	virtual void SetValue(const char * _value){	return BiBxmlNodeImpl::SetValue(_value);}\
	virtual const char * Value() const{return BiBxmlNodeImpl::Value();}\
	virtual DocType GetXmlType(){return DT_BIN;}\
	
class BiBxmlElementImpl;
class BiBxmlNodeImpl: public BiXmlNode
{
public:
	enum BNode_Type{
		BXML_NODE = 0,
		BXML_ELEMENT,
		BXML_ATTRIBUTE,
		BXML_TEXT,
		BXML_COMMENT,
		BXML_DOCUMENT,
		BXML_DECLARATION,
		BXML_UNKNOWN,
	};
protected:
	//节点名称
	std::string m_name;

	//子节点
//	enum {MAX_CHILDREN=128};
//	BiBxmlNodeImpl* m_children[MAX_CHILDREN];
//	int m_child_count;
	std::vector<BiBxmlNodeImpl*> m_children;
	//父节点
	BiBxmlNodeImpl* m_parent;
	//下一个兄弟节点
	BiBxmlNodeImpl* m_next;
	//上一个兄弟节点
	BiBxmlNodeImpl* m_previous;

	void ReleaseChildren();
	virtual void Release() = 0;
public:
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
	static void PutString( const std::string&str, std::ostream* out );
	static void PutString( const std::string& str, std::string* out );
	virtual void StreamOut( std::ostream* stream )  = 0;
public:
	BiBxmlNodeImpl();
	virtual ~BiBxmlNodeImpl();

	BiBxmlNodeImpl* _FirstChild();
	BiBxmlNodeImpl* _FirstNonAttribute();

	const BiBxmlNodeImpl* _FirstNonAttribute() const;
	BiBxmlNodeImpl* _NextSibling(){	return m_next;}
	BiBxmlElementImpl* _FirstChildElement();
	BiBxmlElementImpl* _NextSiblingElement();
	void AddChild(BiBxmlNodeImpl* child);
	virtual const BNode_Type Type()const { return BXML_NODE;};

	virtual void Clear();	
	virtual const BiXmlNode* FirstChild()const;
	virtual BiXmlNode* FirstChild();
	virtual const BiXmlNode* FirstChild( const char * value ) const;
	virtual BiXmlNode* FirstChild( const char * value ) ;
	virtual const BiXmlElement* FirstChildElement()	const;
	virtual BiXmlElement* FirstChildElement();
	virtual const BiXmlElement* FirstChildElement( const char * value ) const;
	virtual BiXmlElement* FirstChildElement( const char * value );
	virtual const BiXmlNode* NextSibling() const;
	virtual BiXmlNode* NextSibling() ;
	virtual const BiXmlNode* NextSibling( const char * _value) const;
	virtual BiXmlNode* NextSibling( const char * _value) ;
	virtual const BiXmlElement* NextSiblingElement() const;
	virtual BiXmlElement* NextSiblingElement();
	virtual const BiXmlElement* NextSiblingElement( const char * _value) const;
	virtual BiXmlElement* NextSiblingElement( const char * _value);
	virtual const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const;
	virtual BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous );
	virtual const BiXmlDocument* GetDocument() const;
	virtual BiXmlDocument* GetDocument();
	virtual const BiXmlNode* Parent() const{return m_parent;};
	virtual BiXmlNode* InsertEndChild( const BiXmlNode& addThis );
	virtual BiXmlNode* InsertEndChild( const char* name );
	virtual BiXmlNode* LinkEndChild( BiXmlNode* addThis );	
	virtual bool RemoveChild( BiXmlNode* removeThis );
	virtual void SetValue(const char * _value){m_name = _value;};
	virtual const char * Value() const{return m_name.c_str();};
	virtual DocType GetXmlType(){return DT_BIN;};

	virtual const BiXmlElement*     ToElement()const {return NULL;};
	virtual BiXmlElement*           ToElement(){return NULL;};
	virtual const BiXmlText*        ToText()const{return NULL;};
	virtual BiXmlText*				ToText(){return NULL;};
	
	virtual BiXmlNode* Clone() const = 0;
};

class BiBxmlAttributeImpl;

class BiBxmlElementImpl : public BiBxmlNodeImpl, public BiXmlElement
{
public:
	///----------接口实现-----
	BIBXML_PUBLIC_INHERIT
	virtual const BiXmlText* ToText()const{	return BiBxmlNodeImpl::ToText();}
	virtual BiXmlText* ToText(){	return BiBxmlNodeImpl::ToText();}
	virtual const BiXmlElement* ToElement()const{return this;}
	virtual BiXmlElement* ToElement(){return this;}
	virtual void StreamOut( std::ostream* stream );
public:
	BiBxmlElementImpl();
	BiBxmlElementImpl(const char* name);
	virtual BiXmlNode* Clone() const;
	virtual const BNode_Type Type() const { return BXML_ELEMENT;}
	virtual const char* Attribute(const char* name)const;
	virtual const char* Attribute(const char* name, int* i)const;
	virtual const char* Attribute(const char* name, double* d)const;

	virtual const char* GetText() const;
	virtual bool SetText(const char* strText);
	virtual bool AddText(const char* strText);

	virtual void SetAttribute( const char* name, const char * _value );
	virtual void SetAttribute( const char * name, int value );
	virtual void SetDoubleAttribute( const char * name, double value );

	virtual int QueryIntAttribute( const char* name, int* _value ) const;
	virtual int QueryFloatAttribute( const char* name, float* _value ) const;

	BiBxmlAttributeImpl* FirstAttribute();

	virtual void Release();

protected:
	//节点值
	std::string m_strText;
};
class BiBxmlDocumentImpl;

class BinXMLReader
{
public:
	enum {DICT_BUFLEN = 8*16*string_dictionary::MAX_STRING};
protected:
	BiBxmlDocumentImpl* m_owndoc;
	string_dictionary m_dict;
	xml_buffer* m_buff;
	char* m_dictbuf;
	char* m_content_buf;
protected:
	void ReadTextOnly(const char*& text);

	void ReadBinXMLValue(BiBxmlNodeImpl* parent, const BXML_TYPE& type);
	void ReadBinXMLAttribute(BiBxmlNodeImpl* parent);
	void ReadBinXMLAttributeValue(BiBxmlAttributeImpl* attribute, const BXML_TYPE& type);
	void ReadBinXMLElementValue(BiBxmlElementImpl* element, const BXML_TYPE& type);
	void ReadBinXMLElement(BiBxmlNodeImpl* parent, int depth);
	void ReadBinXMLDocument();
public:

	BinXMLReader() : m_buff(0), m_content_buf(0), m_owndoc(NULL), m_dictbuf(0)
	{
		//m_dictbuf = new char[DICT_BUFLEN];
	}
	~BinXMLReader()
	{
		if(m_dictbuf)
		{
			delete[] m_dictbuf;
			m_dictbuf = NULL;
		}
		if (m_buff)
		{
			delete m_buff;
			m_buff = NULL;
		}
		if (m_content_buf)
		{
			delete[] m_content_buf;
			m_content_buf = NULL;
		}
	}
	bool LoadFromFile(const char* file);
	bool LoadFromFile(FILE* file);
	bool LoadFileFromPack(const char* filename);
	void ReadBinXML();
	void SetOwner(BiBxmlDocumentImpl* doc) {m_owndoc = doc;};
};

class BinXMLWriter
{
	char* m_buff;
	int m_length;
	int m_write_pos;
	string_dictionary* m_dict;
public:
	enum{WRITE_BUFF = 8 * 1024 * 1024};
	BinXMLWriter(int l, string_dictionary* dict)
	{
		m_dict = dict;
		m_write_pos = 0;
		m_length = l;
		m_buff = new char[l];
	}
	~BinXMLWriter()
	{
		if (m_buff)
		{
			delete []m_buff;
			m_buff = NULL;
		}
	}
protected:
	void write_next(const void* src, int len)
	{
		if(len > m_length-m_write_pos)
		{
			throw_exception();
		}
		memcpy(m_buff+m_write_pos, src, len);
		m_write_pos+=len;
	}
public:
	void throw_exception()
	{
		xml_io_exception e;
		e.pos = this->position();
		throw e;
	}
	int position()
	{
		return m_write_pos;
	}
	char* buffer()
	{
		return m_buff;
	}
	void write(BXML_TYPE data)
	{
		write_next(&data, sizeof(char));
		//printf("%c",data);
		reader_print("%c",data);
	}
	void write(const char& data)
	{
		write(BT_BYTE);
		write_next(&data, sizeof(data));
		//printf("%d",data);
		reader_print("%d",data);
	}
	void write(const short& data)
	{
		write(BT_WORD);
		write_next(&data, sizeof(data));
		//printf("%d",data);
		reader_print("%d",data);
	}
	void write(const int& data)
	{
		write(BT_DWORD);
		write_next(&data, sizeof(data));
		//printf("%d",data);
		reader_print("%d",data);
	}
	void write(const float& data)
	{
		write(BT_FLOAT);
		write_next(&data, sizeof(data));
		//printf("%f",data);
		reader_print("%f",data);
	}
	void write(const char* data)
	{
		short index = m_dict->add_string(data);
		write(BT_INDEXED_STRING);
		write_next(&index, sizeof(index));
		//printf("%d",index);
		//printf("[%s]",data);
		reader_print("%d",index);
		reader_print("[%s]",data);
	}
	void write_string_only(const char* data)
	{
		short index = m_dict->add_string(data);
		write_next(&index, sizeof(index));
		/*printf("%d",index);
		printf("[%s]",data);*/
		reader_print("%d",index);
		reader_print("[%s]",data);
	}
};

class BiBxmlDocumentImpl : public BiBxmlNodeImpl, public BiXmlDocument
{
public:
	BIBXML_PUBLIC_INHERIT
	virtual const BiXmlElement*     ToElement()const{return BiBxmlNodeImpl::ToElement();}
	virtual BiXmlElement*           ToElement(){return BiBxmlNodeImpl::ToElement();}
	virtual const BiXmlText*        ToText()const{return BiBxmlNodeImpl::ToText();}
	virtual BiXmlText* ToText(){	return BiBxmlNodeImpl::ToText();}
public:
	virtual void StreamOut( std::ostream* stream );
	BiBxmlDocumentImpl();
	~BiBxmlDocumentImpl();
	virtual BiXmlNode* Clone() const;
	virtual bool LoadFile( const char* filename, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN );
	virtual bool LoadFile( FILE* file, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN);
	virtual const BiXmlElement* RootElement() const;
	virtual BiXmlElement* RootElement();
	virtual bool SaveFile( const char * filename);
	virtual const char * ErrorDesc() const;
	virtual bool InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone);
	virtual const char* ParseXmlStr(const char* strContent);
	virtual const char* Parse(const char* strContent);
	virtual bool Error()const;

	virtual const BNode_Type Type() const { return BXML_DOCUMENT;}
	virtual void Release();

protected:
	void save_attribute(BiBxmlElementImpl* pElement, unsigned int indent);
	void save_binary(BiBxmlNodeImpl* parent, int indent);
	void save_to_file(const char* filename);
	int GetDictBuffLength(){return BinXMLReader::DICT_BUFLEN;}

	/*
	* m_header、m_dict、m_dictbuf均是在保持二进制xml的时候使用
	*/
	BinXML_Header m_header;
	//std::string m_file;
	string_dictionary m_dict;
	char* m_dictbuf;

	BinXMLReader m_reader;
	BinXMLWriter* m_writer;
};
class BiBxmlAttributeImpl : public BiBxmlNodeImpl
{
public:
	virtual void StreamOut( std::ostream* stream );
	BiXmlNode* Clone()const;
	const char* AttValue()const {return m_value.c_str();};
	void  SetAttValue(const char* strValue){m_value = strValue;};
	BiBxmlAttributeImpl* NextAttribute();
	virtual const BNode_Type Type() const { return BXML_ATTRIBUTE;}
protected:
	virtual void Release();
	std::string m_value;
};
class BiBxmlTextImpl : public BiBxmlNodeImpl
{
	virtual void StreamOut( std::ostream* stream );
	virtual void Release();
};

#endif