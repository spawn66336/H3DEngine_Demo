//这个文件包含在各个VCPROJ里，根据情况进行设置，打开new重载
//目前client_front client_ui client_gameround 三个工程设置了 _NEDMALLOC
//#ifdef _NEDMALLOC

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

	#ifdef NDEBUG
		#ifndef _FINAL_RELEASE		// client ui 使用另一种方式进行vld的包含
			#ifndef CLIENT_UI
				//#define VLD_FORCE_ENABLE 1
				//#include <third_party/vld/include/vld.h>
			#endif
		#endif
	#endif

	#ifndef __UI_LAUNCHER
		#ifndef EDITOR
			#ifndef STAR_SERVER
				#ifndef _DEBUG_TEST
					#include "include/nedmalloc.h"
					#include <map>
					#include <Windows.h>
					#ifndef MEMORY_TEST_ID
					#define  MEMORY_TEST_ID 1
					#endif
					using namespace nedalloc;
#ifdef USE_MEM_PROFILER
					#include <third_party/MemoryProfilerDPLoader/MemoryProfilerDPLoader.h>
#endif
					namespace
					{

						inline void* memoryAlloc(size_t size)
						{	
							return nedmalloc(size);	// 这里请按需配置
						}

						inline void memoryFree(void* p)
						{	
							nedfree(p);	// 这里请按需配置
						}
					}

					void* operator new(size_t size)
					{
#ifdef USE_MEM_PROFILER
						 

						if (size == 0) {
							size = 1;
						}
						static bool bHasLoadDll=false;
						if(bHasLoadDll==false)
						{
							bool re=MemoryProfilerDPLoader::init();
							if(re==false)
							{
								throw "init fail";
							}
							bHasLoadDll=true;
						}
						for(;;) {
							void* p = memoryAlloc(size);
							if (p != 0) {
								CONTEXT c;
								memset(&c, 0, sizeof(CONTEXT));
								c.ContextFlags = CONTEXT_FULL;
								RtlCaptureContext(&c);
								MemoryProfilerDPLoader::addAllocationInfo(MEMORY_TEST_ID, &c, p, size);
								return p;
							}

							std::new_handler globalhandler = std::set_new_handler(0);
							std::set_new_handler(globalhandler);

							if (globalhandler) {
								(*globalhandler)();
							}
							else {
								throw std::bad_alloc();
							}
						}
#else
						return memoryAlloc(size);
#endif
 
					}

					void operator delete(void* p)
					{
#ifdef USE_MEM_PROFILER
						memoryFree(p);
						MemoryProfilerDPLoader::removeAllocationInfo(MEMORY_TEST_ID, p);
#else
						memoryFree(p);
#endif
					 
					}

					void* operator new[](size_t size)
					{
						return operator new(size);
					}

					void operator delete[](void* p)
					{
						return operator delete(p);
					}
// #ifdef USE_STACKWALKER
// static bool first_new = true;
// #else
// static bool first_new = false;
// #endif
// 
// typedef void (*voidPtrIntVoid)(void* p,int size);
// typedef void (*voidPtrVoid)(void* p);
// typedef void (*voidVoid)();
// typedef bool (*boolVoid)();
// typedef bool (*voidBool)(bool is);
// 
// static voidVoid initWs = NULL;
// static voidPtrIntVoid addPtr = NULL;
// static voidPtrVoid removePtr = NULL;
// static voidVoid dumpFile = NULL;
// static boolVoid isRecord = NULL;
// static voidBool setIsRecord = NULL;
// 
// void dumpMemMap()
// {
// 	if (dumpFile)
// 	{
// 		dumpFile();
// 	}
// }
// 				void* operator new(size_t size)
// 				{
// 					#ifdef USE_STACKWALKER
// 					if (first_new)
// 					{
// 						HINSTANCE handle = LoadLibraryA("StackWalker.dll");
// 						if(handle)
// 						{
// 							initWs = (voidVoid)GetProcAddress(handle,"Init");
// 							addPtr = (voidPtrIntVoid)GetProcAddress(handle,"AddPtr");
// 							removePtr = (voidPtrVoid)GetProcAddress(handle,"RemovePtr");
// 							dumpFile = (voidVoid)GetProcAddress(handle,"WriteLeak");
// 							isRecord = (boolVoid)GetProcAddress(handle,"IsRecord");
// 							setIsRecord = (voidBool)GetProcAddress(handle,"SetIsRecord");
// 							initWs();
// 						}
// 						first_new = false;
// 					}
// 
// 					void* addr = nedmalloc(size);
// 					if (addPtr && isRecord())
// 					{
// 						addPtr(addr,size);
// 					}
// 					return addr;
// 					#endif
// 					return nedmalloc(size);
// 				}
// 				void operator delete(void* p)
// 				{
// #ifdef USE_STACKWALKER
// 					if (removePtr && isRecord())
// 					{
// 						removePtr(p);
// 					}
// #endif
// 					nedfree(p);
// 				}
// 				void *operator new[]( size_t size ) {
// #ifdef USE_STACKWALKER
// 					void* addr = nedmalloc(size);
// 					if (addPtr && isRecord() )
// 						addPtr(addr,size);
// 					return addr;
// #endif
// 					return nedmalloc(size);
// 				}
// 				void operator delete[]( void *p ) {
// #ifdef USE_STACKWALKER
// 		
// 					if (removePtr && isRecord)
// 					{
// 						removePtr(p);
// 					}
// #endif
// 					nedfree(p);
// 				}
// 
// 
// #ifdef USE_STACKWALKER
// 				void enableStackWalker(bool enable)
// 				{
// 					setIsRecord(enable);
// 				}
// #endif
				#endif
			#endif
		#endif
	#endif
#endif

//#endif