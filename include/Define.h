#ifndef _Define_Headfile_
#define _Define_Headfile_

/////////////////////////////////////////////////////////////////////////////////
// ���Ͷ���
typedef int					BOOL;
typedef uint8_t				BYTE;
typedef char				CHAR;
typedef char *				LPCSTR;
typedef unsigned int        UINT;
typedef unsigned int        DWORD;

/////////////////////////////////////////////////////////////////////////////////
#define FALSE				0
#define TRUE				1

/////////////////////////////////////////////////////////////////////////////////
// #define NULL				(void*)0L

/////////////////////////////////////////////////////////////////////////////////
#define Sleep(ms)			usleep(1000*ms)

/////////////////////////////////////////////////////////////////////////////////
#endif
