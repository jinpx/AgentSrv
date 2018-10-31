#include <TraceService.h>

/////////////////////////////////////////////////////////////////////////////////

//��Ϣ�ṹ
struct MSGITEM
{
	BYTE	nType;
	CHAR	szTime[100];
	CHAR	szText[256];
};

/////////////////////////////////////////////////////////////////////////////////
struct tagTraceService
{
	CRITICAL_SECTION		cs;										// ����Ϣ
	std::vector<MSGITEM *>	vecInfoArray;							// ��Ϣ��Ϣ
	std::vector<MSGITEM *>	vecNormalArray;							// ��ͨ��Ϣ
	std::vector<MSGITEM *>	vecWarningArray;						// ������Ϣ
	std::vector<MSGITEM *>	vecExceptionArray;						// �쳣��Ϣ
	std::vector<MSGITEM *>	vecDebugArray;							// ������Ϣ
};

/////////////////////////////////////////////////////////////////////////////////
// ������
static tagTraceService * g_TraceService = NULL;

/////////////////////////////////////////////////////////////////////////////////
// ��������

// ���캯��
CTraceService::CTraceService()
{
	if (g_TraceService==NULL) {
		g_TraceService = new tagTraceService;
		if (g_TraceService!=NULL) {
			InitializeCriticalSection( &g_TraceService->cs );
		}
	}
}

// ��������
CTraceService::~CTraceService()
{
	if (g_TraceService!=NULL) {
		DeleteCriticalSection( &g_TraceService->cs );
		delete g_TraceService;
		g_TraceService = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// �����־

BOOL traceService()
{
	if (g_TraceService == NULL)
		return FALSE;
	
	// ����
	MSGITEM item;
	std::vector<MSGITEM *>::iterator Iter;

	// ��ס����
	EnterCriticalSection( &g_TraceService->cs );

	// �洢����
	if (vecInfoArray.size())
	{
		for(Iter = vecInfoArray.begin(); Iter != vecInfoArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ��%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecInfoArray.clear();
	}

	if (vecNormalArray.size())
	{
		for(Iter = vecNormalArray.begin(); Iter != vecNormalArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ��%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecNormalArray.clear();
	}

	if (vecInfoArray.size())
	{
		for(Iter = vecWarningArray.begin(); Iter != vecWarningArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ��%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecWarningArray.clear();
	}

	if (vecInfoArray.size())
	{
		for(Iter = vecExceptionArray.begin(); Iter != vecExceptionArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ��%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecExceptionArray.clear();
	}

	if (vecDebugArray.size())
	{
		for(Iter = vecDebugArray.begin(); Iter != vecDebugArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ��%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecDebugArray.clear();
	}

	// �뿪������
	LeaveCriticalSection( &g_TraceService->cs );
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////
//������־

BOOL traceString(BYTE nType, LPCSTR szMsg, ...)
{
	if ( g_TraceService == NULL )
		return;
		
	MSGITEM * pMsg = new MSGITEM;
	assert(pMsg);

	// ��������
	if ( pMsg == NULL ) {
		return;
	}

	// ��������
	pMsg->nType = nType;

	// ����ʱ��
	time_t now = time(NULL);
	struct tm* sysTime = localtime(&now);
	sprintf(pMsg->szTime,"%d.%d.%d %d:%d:%d", sysTime->tm_year+1900, sysTime->tm_mon+1, 
		sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);

	va_list args;
	va_start(args, szMsg);
	vsnprintf( pMsg->szText, 256, szMsg, args );
	va_end(args);

	// ��ס����
	EnterCriticalSection( &g_TraceService->cs );

	// �洢����
	if (nType==TraceLevel_Info) {	
		g_TraceService->vecInfoArray.push_back(pMsg);
		return TRUE;
	}
	else if (nType==TraceLevel_Info) {	
		g_TraceService->vecNormalArray.push_back(pMsg);
		return TRUE;
	}
	else if (nType==TraceLevel_Info) {	
		g_TraceService->vecWarningArray.push_back(pMsg);
		return TRUE;
	}
	else if (nType==TraceLevel_Info) {	
		g_TraceService->vecExceptionArray.push_back(pMsg);
		return TRUE;
	}
	else if (nType==TraceLevel_Info) {	
		g_TraceService->vecDebugArray.push_back(pMsg);
		return TRUE;
	}

	// �뿪������
	LeaveCriticalSection( &g_TraceService->cs );
	return TRUE;
}