#include <Utility.h>
#include <TraceService.h>

/////////////////////////////////////////////////////////////////////////////////
// ��־�߳�
bool threadShutdown = false;
pthread_t threadPrintData = 0L;
void * threadTraceService( void * param );

/////////////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó�ʼ��
bool mainInitialize()
{
	threadShutdown = true;
	pthread_create( &threadPrintData, 0, threadTraceService, 0);
}

// ��־����
CTraceService tarceService;
/////////////////////////////////////////////////////////////////////////////////

// ������
int main(int agrv, char** agrc)
{
	// ��ʼ��
	mainInitialize();
	
	static UINT nPrintCount = 0;

	// ��ʼ����
	while( true )
	{
		nPrintCount++;
		traceString(TraceLevel_Info,"print %d", nPrintCount);
		Sleep(1500);
	}

	printf( "%d( %s )\n", agrv, agrc[0]);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��ӡ��־�߳�
void * threadTraceService( void * param )
{	
	// ��ʼ����
	while( threadShutdown )
	{
		BOOL bRet = traceUpdate();
		if (bRet==TRUE) 
		{
			Sleep(3000L);
			continue;
		}
		return NULL;
	}
	return NULL;
}