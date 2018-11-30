#ifndef _Define_Headfile_
#define _Define_Headfile_

/////////////////////////////////////////////////////////////////////////////////
// 类型定义

typedef int64_t						INT64;
typedef int32_t						INT32;
typedef int16_t						INT16;
typedef int8_t						INT8;
typedef uint64_t					UINT64;
typedef uint32_t					UINT32;
typedef uint16_t					UINT16;
typedef uint8_t						UINT8;
typedef int							SOCKET;
typedef int							BOOL;

typedef uint8_t						BYTE;
typedef uint16_t					WORD;
typedef uint32_t					DWORD, DWORD_PTR;
typedef uint16_t					USHORT;

typedef float						FLOAT;
typedef double						DOUBLE;
typedef int							INT;
typedef unsigned int				UINT;
typedef char						CHAR;
typedef wchar_t						TCHAR;
typedef short						SHORT;
typedef unsigned short				USHORT;
typedef char						*LPSTR, *PSTR;
typedef const char					*LPCSTR, *PCSTR;

typedef void						VOID, *LPVOID;
typedef long						LONG;
typedef unsigned long				ULONG;

/////////////////////////////////////////////////////////////////////////////////
typedef struct						sockaddr_in SOCKADDR_IN;
typedef struct						sockaddr SOCKADDR;

/////////////////////////////////////////////////////////////////////////////////
#define INVALID_SOCKET				-1
#define	SOCKET_ERROR				-1

/////////////////////////////////////////////////////////////////////////////////
#define FALSE						0
#define TRUE						1

/////////////////////////////////////////////////////////////////////////////////
// #define NULL						(void*)0L

/////////////////////////////////////////////////////////////////////////////////
#define Sleep(ms)					usleep(1000*ms)

/////////////////////////////////////////////////////////////////////////////////
DWORD  GetTickCount();				// 计时器

/////////////////////////////////////////////////////////////////////////////////
#define SAFE_DELETE( p)				{ if ( p) { delete ( p); ( p) = NULL; } }
#define SAFE_DELETE_ARRAY( p)		{ if ( p) { delete[] ( p); ( p) = NULL; } }
#define SAFE_RELEASE( p)			{ if ( p) { ( p)->Release(); ( p) = NULL; } }
#define	SAFE_RELEASENDELETE(p)		if (p) { (p)->Release(); delete (p); (p) = NULL; }

/////////////////////////////////////////////////////////////////////////////////
#define MAX_TIMEDATA_SIZE			( 20 )

/////////////////////////////////////////////////////////////////////////////////
#define MAKELONG(a, b)				((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))

/////////////////////////////////////////////////////////////////////////////////
const int MAX_ID_LEN				= 24;
const int MAX_PD_LEN				= 24;
const int MAX_IP_LEN				= 32;

/////////////////////////////////////////////////////////////////////////////////
#ifndef max
#define max(a,b)					(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)					(((a) < (b)) ? (a) : (b))
#endif

/////////////////////////////////////////////////////////////////////////////////
#endif
