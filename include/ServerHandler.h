#ifndef _ServerHandler_Headfile_
#define _ServerHandler_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ThreadLock.h"
#include "CircuitQueue.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
#define MAX_IO_WORKER_THREAD	16
#define EXTRA_ACCEPTEX_NUM		10
#define SOCKET_HOLDER_SIZE  	1024

/////////////////////////////////////////////////////////////////////////////////
class CSession;														// 会话活动
class CNetworkObject;												// 网络对象
class CUserServer;													// 用户连接
class CGameServer;													// 游戏服务
class CLogonServer;													// 登录服务

/////////////////////////////////////////////////////////////////////////////////
// IO句柄

class CServerHandler
{
public:
	DWORD				m_dwKey;
	int					m_epoll;									// epoll fd 
	DWORD				m_numIoThreads;								// IO 
	pthread_t			m_hIoThread[MAX_IO_WORKER_THREAD];			// IO 
	pthread_t			m_hEpollThread;

public:
	// 构造函数
	CServerHandler();
	// 析构函数
	virtual ~CServerHandler();

public:
	// 服务器连接
	DWORD Connect(CNetworkObject * pNetworkObject, char * pszIP, WORD wPort);
	// 监听用户
	BOOL StartListen(char * pszIP, WORD wPort);

public:
	// 添加事件
	void AddIoEvent(struct epoll_event* pEvent);
	// 修改事件
	int	 ModEpollEvent(Session *pSession, DWORD nEvent );
	// 添加事件
	int	 AddEpollEvent(Session *pSession );
	// 删除事件
	void DelEpollEvent(Session *pSession );
};

#endif
	