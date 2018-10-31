#include <Utility.h>
#include <TraceService.h>
	

/////////////////////////////////////////////////////////////////////////////////
// ��־�߳�
bool threadShutdown = false;
pthread_t threadWriteFile = NULL;
void * threadTraceService( void * param );

/////////////////////////////////////////////////////////////////////////////////
// ������Ӧ�ó�ʼ��
bool mainInitialize()
{
	threadShutdown = true;
	pthread_create( &threadWriteFile, 0, threadTraceService, 0);
}

/////////////////////////////////////////////////////////////////////////////////
// ������
int main(int agrv, char** agrc)
{
	// ��ʼ��
	mainInitialize();

	printf( "%d( %s )\n", agrv, agrc[0]);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ��ӡ��־�߳�
void * threadTraceService( void * param )
{
	// ��ʼ������
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