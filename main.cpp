#include <Utility.h>
#include <TraceService.h>

/////////////////////////////////////////////////////////////////////////////////
// ��־�߳�
bool threadShutdown = false;
pthread_t threadPrintData = NULL;
void * threadTraceService( void * param );

/////////////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó�ʼ��
bool mainInitialize()
{
	threadShutdown = true;
	pthread_create( &threadPrintData, 0, threadTraceService, 0);
}

/////////////////////////////////////////////////////////////////////////////////
// ������
int main(int agrv, char** agrc)
{
	// ��ʼ��
	mainInitialize();
	
	// ��ʼ����
	while( true )
	{
		traceString(TraceLevel_Info,"print %d", nCountPrint);
		if (!bRet) {
			Sleep(1500);
			continue;
		}
	}

	printf( "%d( %s )\n", agrv, agrc[0]);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��ӡ��־�߳�
void * threadTraceService( void * param )
{	
	// ��־����
	CTraceService tarceService;

	// ��ʼ����
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