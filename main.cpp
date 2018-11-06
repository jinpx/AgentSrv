#include <Utility.h>
#include <TraceService.h>

/////////////////////////////////////////////////////////////////////////////////
// 日志线程
bool threadShutdown = false;
pthread_t threadPrintData = NULL;
void * threadTraceService( void * param );

/////////////////////////////////////////////////////////////////////////////////
// 主函数应用初始化
bool mainInitialize()
{
	threadShutdown = true;
	pthread_create( &threadPrintData, 0, threadTraceService, 0);
}

/////////////////////////////////////////////////////////////////////////////////
// 主函数
int main(int agrv, char** agrc)
{
	// 初始化
	mainInitialize();
	
	static UINT nPrintCount = 0;

	// 开始运行
	while( true )
	{
		nCountPrint++;
		traceString(TraceLevel_Info,"print %d", nCountPrint);
		Sleep(1500);
	}

	printf( "%d( %s )\n", agrv, agrc[0]);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// 打印日志线程
void * threadTraceService( void * param )
{	
	// 日志参数
	CTraceService tarceService;

	// 开始运行
	while( threadShutdown )
	{
		BOOL bRet = traceUpdate();
		if (!bRet) {
			Sleep(3000);
			continue;
		}
	}
	return 0;
}