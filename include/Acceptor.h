#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#pragma once
/////////////////////////////////////////////////////////////////////////////////
class CSession;
class CServerHandler;

/////////////////////////////////////////////////////////////////////////////////
typedef struct tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

/////////////////////////////////////////////////////////////////////////////////
// ������
class CAcceptor
{
	// �����߳�
	friend void * accept_thread(void* param );
	
	// ��������
private:
	CServerHandler				  * m_pHandler;					// ������
	SOCKET							m_listenSocket;				// �������
	SOCKADDR_IN						m_sockaddr;					// ������ַ
	int								m_bShutdown	;				// ��������
	pthread_t						m_hAcceptThread;			// �����߳�

public:
	// ���캯��
	CAcceptor();
	// ��������
	~CAcceptor();
	
	// ��ʼ������
	void Init( CServerHandler *pHandler  );
	// ��ʼ����
	BOOL StartListen( char *pIP, WORD wPort );
	// �Ƿ�ر�
	void Shutdown();
	// �Ƿ����
	inline BOOL IsListening() { 
		return m_listenSocket != INVALID_SOCKET; 
	}
	// ��ȡ����Socket
	inline SOCKET GetListenSocket() { 
		return m_listenSocket; 
	}
};

#endif // _ACCEPTOR_H_
	