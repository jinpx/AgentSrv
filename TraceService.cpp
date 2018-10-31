#include <TraceService.h>

/////////////////////////////////////////////////////////////////////////////////

//消息结构
struct MSGITEM
{
	BYTE	nType;
	CHAR	szTime[100];
	CHAR	szText[256];
};

/////////////////////////////////////////////////////////////////////////////////
struct tagTraceService
{
	pthread_mutex_t			threadMutex;							// 锁信息
	std::vector<MSGITEM *>	vecInfoArray;							// 信息消息
	std::vector<MSGITEM *>	vecNormalArray;							// 普通消息
	std::vector<MSGITEM *>	vecWarningArray;						// 警告消息
	std::vector<MSGITEM *>	vecExceptionArray;						// 异常消息
	std::vector<MSGITEM *>	vecDebugArray;							// 调试消息
};

/////////////////////////////////////////////////////////////////////////////////
// 主函数
static tagTraceService * g_TraceService = NULL;

/////////////////////////////////////////////////////////////////////////////////
// 跟踪类型

// 构造函数
CTraceService::CTraceService()
{
	if (g_TraceService==NULL) {
		g_TraceService = new tagTraceService;
	}
}

// 析构函数
CTraceService::~CTraceService()
{
	if (g_TraceService!=NULL) {
		delete g_TraceService;
		g_TraceService = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 输出日志

BOOL traceUpdate()
{
	if (g_TraceService == NULL)
		return FALSE;
	
	// 变量
	MSGITEM item;
	std::vector<MSGITEM *>::iterator Iter;

	// 锁住数据
	pthread_mutex_lock(&g_TraceService->threadMutex);
	
	// 存储数据
	if (vecInfoArray.size())
	{
		for(Iter = vecInfoArray.begin(); Iter != vecInfoArray.end(); Iter++)
		{
			item.nType = (*m_Iter)->nType;
			strcpy(item.szTime, (*m_Iter)->szTime);
			strcpy(item.szText, (*m_Iter)->szText);
			printf("%s ：%s\n", item.szTime,item.szText);
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
			printf("%s ：%s\n", item.szTime,item.szText);
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
			printf("%s ：%s\n", item.szTime,item.szText);
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
			printf("%s ：%s\n", item.szTime,item.szText);
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
			printf("%s ：%s\n", item.szTime,item.szText);
			delete (LOGITEM *)(*m_Iter);
		}
		vecDebugArray.clear();
	}

	// 离开数据区
	pthread_mutex_unlock(&g_TraceService->threadMutex);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////
//添加日志

BOOL traceString(BYTE nType, LPCSTR szMsg, ...)
{
	if ( g_TraceService == NULL )
		return;
		
	MSGITEM * pMsg = new MSGITEM;
	assert(pMsg);

	// 数据类型
	if ( pMsg == NULL ) {
		return;
	}

	// 构造类型
	pMsg->nType = nType;

	// 构造时间
	time_t now = time(NULL);
	struct tm* sysTime = localtime(&now);
	sprintf(pMsg->szTime,"%d.%d.%d %d:%d:%d", sysTime->tm_year+1900, sysTime->tm_mon+1, 
		sysTime->tm_mday, sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);

	va_list args;
	va_start(args, szMsg);
	vsnprintf( pMsg->szText, 256, szMsg, args );
	va_end(args);

	// 锁住数据
	pthread_mutex_lock(&g_TraceService->threadMutex);

	// 存储数据
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

	// 离开数据区
	pthread_mutex_unlock(&g_TraceService->threadMutex);
	return TRUE;
}
