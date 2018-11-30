#ifndef _Connector_Headfile_
#define _Connector_Headfile_

#pragma once
#include <Network.h>

/////////////////////////////////////////////////////////////////////////////////
class CSession;
class CServerHandler;

/////////////////////////////////////////////////////////////////////////////////
#define MAX_CONNECT_THREAD	16

/////////////////////////////////////////////////////////////////////////////////
// 接受器

class CConnector
{
	// 接受线程
	friend void * connect_thread(void* param );
	
	// 参数变量
private:
	CSession		  *	m_pSession;									// 会话连接
	pthread_t			m_hThread;									// 线程活动
	BOOL				m_bShutdown;								// 关闭线程
	sem_t				m_semConnect;								// 连接信号

public:
	// 构造函数
	CConnector();
	// 析构函数
	~CConnector();
	
	// 初始化数据
	void Init( CServerHandler *pHandler  );
	// 开始连接
	void Connect( CSession * m_Session );
	// 是否关闭
	void Shutdown();
};

#endif // _ACCEPTOR_H_
	