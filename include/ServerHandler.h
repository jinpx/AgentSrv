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
class CSession;														// �Ự�
class CNetworkObject;												// �������
class CUserSession;													// �û�����
class CGameSession;													// ��Ϸ����
class CLogonSession;												// ��¼����

/////////////////////////////////////////////////////////////////////////////////
// IO���

class CServerHandler
{
	// ��������
public:
	
	// ������
	CUserSession		*m_pUserSession;							// �û��Ự
	CLogonSession		*m_pLogonSession;							// ��¼�Ự
	CGameSession		*m_pGameSession;							// ��Ϸ�Ự
	BOOL				m_bShutdown;

	// �¼�����
	DWORD				m_dwKey;
	int					m_epoll;									// epoll fd 
	DWORD				m_numIoThreads;								// IO 
	pthread_t			m_hIoThread[MAX_IO_WORKER_THREAD];			// IO 
	pthread_t			m_hEpollThread;

	CCircuitQueue<struct epoll_event> * m_pEvents;					// �����¼�
	CThreadLock			m_lockEvents;								// ��������
	CThreadCond			m_condEvents;								// ����Լ��

public:
	// ���캯��
	CServerHandler();
	// ��������
	virtual ~CServerHandler();

public:
	// ��ʼ������
	void Init();
	// �����û�
	BOOL StartListen(char * pszIP, WORD wPort);
	// ��������
	void Update();
	// �رշ���
	void Shutdown();
	// �Ƿ�����
	BOOL IsRunning() { return !m_bShutdown; }
	// ����������
	DWORD Connect(CNetworkObject * pNetworkObject, char * pszIP, WORD wPort);

public:
	// ����¼�
	void AddIoEvent(struct epoll_event * pEvent);
	// �޸��¼�
	int	 ModEpollEvent(CSession *pSession, DWORD nEvent );
	// ����¼�
	int	 AddEpollEvent(CSession *pSession );
	// ɾ���¼�
	void DelEpollEvent(CSession *pSession );
};

#endif
	