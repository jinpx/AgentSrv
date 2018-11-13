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
class CUserServer;													// �û�����
class CGameServer;													// ��Ϸ����
class CLogonServer;													// ��¼����

/////////////////////////////////////////////////////////////////////////////////
// IO���

class CServerHandler
{
public:
	DWORD				m_dwKey;
	int					m_epoll;									// epoll fd 
	DWORD				m_numIoThreads;								// IO 
	pthread_t			m_hIoThread[MAX_IO_WORKER_THREAD];			// IO 
	pthread_t			m_hEpollThread;

public:
	// ���캯��
	CServerHandler();
	// ��������
	virtual ~CServerHandler();

public:
	// ����������
	DWORD Connect(CNetworkObject * pNetworkObject, char * pszIP, WORD wPort);
	// �����û�
	BOOL StartListen(char * pszIP, WORD wPort);

public:
	// ����¼�
	void AddIoEvent(struct epoll_event* pEvent);
	// �޸��¼�
	int	 ModEpollEvent(Session *pSession, DWORD nEvent );
	// ����¼�
	int	 AddEpollEvent(Session *pSession );
	// ɾ���¼�
	void DelEpollEvent(Session *pSession );
};

#endif
	