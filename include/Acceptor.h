#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#pragma once
/////////////////////////////////////////////////////////////////////////////////
class CSession;
class CServerHandler;

/////////////////////////////////////////////////////////////////////////////////
typedef struct tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

/////////////////////////////////////////////////////////////////////////////////
// 接受器
class CAcceptor
{
	// 接受线程
	friend void * accept_thread(void* param );
	
	// 参数变量
private:
	CServerHandler				  * m_pHandler;					// 服务句柄
	SOCKET							m_listenSocket;				// 监听句柄
	SOCKADDR_IN						m_sockaddr;					// 监听地址
	int								m_bShutdown	;				// 正常监听
	pthread_t						m_hAcceptThread;			// 监听线程

public:
	// 构造函数
	CAcceptor();
	// 析构函数
	~CAcceptor();
	
	// 初始化数据
	void Init( CServerHandler *pHandler  );
	// 开始监听
	BOOL StartListen( char *pIP, WORD wPort );
	// 是否关闭
	void Shutdown();
	// 是否监听
	inline BOOL IsListening() { 
		return m_listenSocket != INVALID_SOCKET; 
	}
	// 获取监听Socket
	inline SOCKET GetListenSocket() { 
		return m_listenSocket; 
	}
};

#endif // _ACCEPTOR_H_
	