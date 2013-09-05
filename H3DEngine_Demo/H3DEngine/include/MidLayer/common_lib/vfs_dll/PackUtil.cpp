//#include "stdafx.h"
#include "mycrypt.h"
#include "filesystem.h"
#include "stdio.h"
#include <windows.h>
#define MAX_PATH_LENGTH 256
namespace VFS
{


	static char sTemp[MAX_PATH_LENGTH] ;
	static char sTemp2[MAX_PATH_LENGTH] ;
	const char* const  MakePurePathString(const char* s,const char* sReserve,int func)
	{
		if (!s )
			return 0 ;
		if (!s[0])
		{
			sTemp[0]=0 ;
			return sTemp ;
		}

		if (strlen(s)>=MAX_PATH_LENGTH-1)
		{
			memcpy(sTemp,s,MAX_PATH_LENGTH-1) ;
			sTemp[127]=0 ;
		}
		else
			strcpy(sTemp,s) ;
		char* sLw = _strlwr(sTemp) ;

		size_t len = strlen(sTemp);

		for (size_t i=0; i<len; ++i)
		{
			if (sTemp[i] == '/')
				sTemp[i]='\\' ;

			if (i<len-1)
			{
				if (sTemp[i]=='\\' && 
					(sTemp[i+1]=='/' ||	sTemp[i+1]=='\\' )
					)
				{
					//eat it! u fucking shit
					for (size_t j=i+1;j<strlen(sTemp);++j)
						sTemp[j] = sTemp[j+1] ;

					len = strlen(sTemp);

				}
			}
		}		
   

		sLw = sTemp;

		char* sRelativeDir =0 ;
		if (sReserve)
		{
			strcpy(sTemp2,sReserve) ;
			char* sReserveLw = _strlwr(sTemp2) ;
			sRelativeDir= strstr(sLw,sReserveLw) ;
		}

		//there' s no D:\ReserveDir string in s or no Reserve string
		if (!sRelativeDir)
		{
			sRelativeDir = sLw ;
			if (func==1)
				return 0;
		}
		else
			sRelativeDir += strlen(sTemp2) ;

		if (strlen(sRelativeDir)>=MAX_PACK_FILE_NAME)
		{
			assert(!"error! Path name length > 128!!!") ;
		}

		return (const char* const)sRelativeDir ;
	}
	void MakeMD5(const char* sString,md5_sum& md5)
	{
		if (!sString || !sString[0])
			return ;

		hash_state md1 ;
		md5_init(&md1) ;
		md5_process(&md1,(const unsigned char*)sString,(unsigned long)strlen(sString)) ;
		unsigned char out[16] ;
		md5_done(&md1,out) ;
		memcpy(&md5,out,16) ;
	}

	void ReportError(const char* err)
	{
#ifdef _DEBUG

		::MessageBoxA(0,err,0,0);

#endif // _DEBUG
	}

	void ReportErrorEx(const char* str,...)
	{
#ifdef _DEBUG
		char szParsedString[1024];
		va_list va;
		va_start( va, str );
		vsprintf( szParsedString, str, va );
		va_end( va );

		::MessageBoxA(0,szParsedString,0,0);
#endif // _DEBUG
	}



}