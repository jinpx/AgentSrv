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
// ������

class CConnector
{
	// �����߳�
	friend void * connect_thread(void* param );
	
	// ��������
private:
	CSession		  *	m_pSession;									// �Ự����
	pthread_t			m_hThread;									// �̻߳
	BOOL				m_bShutdown;								// �ر��߳�
	sem_t				m_semConnect;								// �����ź�

public:
	// ���캯��
	CConnector();
	// ��������
	~CConnector();
	
	// ��ʼ������
	void Init( CServerHandler *pHandler  );
	// ��ʼ����
	void Connect( CSession * m_Session );
	// �Ƿ�ر�
	void Shutdown();
};

#endif // _ACCEPTOR_H_
	