/*
www.sourceforge.net/projects/tinyxml
Original file by Yves Berquin.

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

/*
 * THIS FILE WAS ALTERED BY Tyge Løvset, 7. April 2005.
 */


#ifndef BIXML_USE_STL

#include "tinystr.h"

//#include "mmgrInclude.h"

// Error value for find primitive
const BiXmlString::size_type BiXmlString::npos = static_cast< size_type >(-1);

// Null rep.
BiXmlString::Rep BiXmlString::nullrep_ = { 0, 0, '\0' };


void BiXmlString::reserve (size_type cap)
{
	if (cap > capacity())
	{
		BiXmlString tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}

void BiXmlString::init(size_type sz, size_type cap)
{
	if (cap)
	{
		// Lee: the original form:
		//	rep_ = static_cast<Rep*>(operator new(sizeof(Rep) + cap));
		// doesn't work in some cases of new being overloaded. Switching
		// to the normal allocation, although use an 'int' for systems
		// that are overly picky about structure alignment.
		const size_type bytesNeeded = sizeof(Rep) + cap;
		const size_type intsNeeded = ( bytesNeeded + sizeof(int) - 1 ) / sizeof( int ); 
		rep_ = reinterpret_cast<Rep*>( new int[ intsNeeded ] );

		rep_->str[ rep_->size = sz ] = '\0';
		rep_->capacity = cap;
	}
	else
	{
		rep_ = &nullrep_;
	}
}

void BiXmlString::quit()
{
	if (rep_ != &nullrep_)
	{
		// The rep_ is really an array of ints. (see the allocator, above).
		// Cast it back before delete, so the compiler won't incorrectly call destructors.
		delete [] ( reinterpret_cast<int*>( rep_ ) );
	}
}


BiXmlString& BiXmlString::assign(const char* str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		BiXmlString tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);
		set_size(len);
	}
	return *this;
}


BiXmlString& BiXmlString::append(const char* str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}


BiXmlString operator + (const BiXmlString & a, const BiXmlString & b)
{
	BiXmlString tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

BiXmlString operator + (const BiXmlString & a, const char* b)
{
	BiXmlString tmp;
	BiXmlString::size_type b_len = static_cast<BiXmlString::size_type>( strlen(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

BiXmlString operator + (const char* a, const BiXmlString & b)
{
	BiXmlString tmp;
	BiXmlString::size_type a_len = static_cast<BiXmlString::size_type>( strlen(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}


#endif	// BIXML_USE_STL
