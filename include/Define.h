#ifndef _Define_Headfile_
#define _Define_Headfile_

/////////////////////////////////////////////////////////////////////////////////
// 类型定义
typedef int64_t				INT64;
typedef int32_t				INT32;
typedef int16_t				INT16;
typedef int8_t				INT8;
typedef uint64_t			UINT64;
typedef uint32_t			UINT32;
typedef uint16_t			UINT16;
typedef uint8_t				UINT8;
typedef int					SOCKET;
typedef int					BOOL;

typedef uint8_t				BYTE;
typedef uint16_t			WORD;
typedef uint32_t			DWORD;
typedef uint16_t			USHORT;

typedef float               FLOAT;
typedef double				DOUBLE;
typedef int                 INT;
typedef unsigned int        UINT;
typedef char				CHAR;
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef char				*LPSTR, *PSTR;
typedef const char			*LPCSTR, *PCSTR;

/////////////////////////////////////////////////////////////////////////////////
typedef struct				sockaddr_in SOCKADDR_IN;
typedef struct				sockaddr SOCKADDR;

/////////////////////////////////////////////////////////////////////////////////
#define INVALID_SOCKET		-1
#define	SOCKET_ERROR		-1

/////////////////////////////////////////////////////////////////////////////////
#define FALSE				0
#define TRUE				1

/////////////////////////////////////////////////////////////////////////////////
// #define NULL				(void*)0L

/////////////////////////////////////////////////////////////////////////////////
#define Sleep(ms)			usleep(1000*ms)

/////////////////////////////////////////////////////////////////////////////////
#endif
