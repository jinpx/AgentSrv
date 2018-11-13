#ifndef _Session_Headfile_
#define _Session_Headfile_

#pragma once
#include "SocketOpt.h"
/////////////////////////////////////////////////////////////////////////////////

class CNetworkObject;
class CServerHandler;
class CSendBuffer;
class CRecvBuffer;

/////////////////////////////////////////////////////////////////////////////////
// �Ự����

class CSession
{
	// ��Ԫ����
	friend void* io_thread( void* param );
	
	// ��������
public:
	CNetworkObject			*m_pNetworkObject;				// �������
	CSendBuffer				*m_pSendBuffer;					// ���ͻ���	
	CRecvBuffer				*m_pRecvBuffer;					// ���ջ���
	SOCKET					m_socket;						// ������
	SOCKADDR_IN				m_sockaddr;						// �����ַ
	DWORD					m_dwLastSyncTick;				// ����ʱ��
	volatile int			m_bRemove;						// �Ƿ��Ƴ�
	DWORD					m_dwTimeOut;					// ��ʱ����
	DWORD					m_dwIndex;						// �Ự�±�
	BOOL					m_bAcceptSocket;				// �Ƿ����
	BOOL					m_bDisconnectOrdered;			// �Ƿ�Ͽ�
	int						m_iCategory;					// Э������
	int						m_iProtocol;					// Э�����
	int						m_iSize;						// �ֽڴ�С
	CThreadLock				m_lockRecv;						// ������
	CThreadLock				m_lockSend;						// ������
	BOOL					m_bCanSend;						// �ɷ���
	UINT64					m_dwTotalRecvBytes;				// �ܹ����յ��������ֽ���
	UINT64					m_dwTotalSendBytes;				// �ܹ����յ��������ֽ���

public:
	// ���캯��
	CSession( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut );
	// ��������
	virtual ~CSession();

	// ��ʼ������
	void					Init();
	// ��������
	BOOL					Send( BYTE *pMsg, WORD wSize );
	// ��չ����
	BOOL					SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	// ��ʼ����
	BOOL					OnSend();
	// ׼������
	BOOL 					PreSend(CServerHandler* pIoHandler);
	// ��������
	BOOL					DoSend(CServerHandler* pIoHandler);
	// ��������
	BOOL					DoRecv();
	// ����������
	SOCKET					CreateSocket();
	// ���������ݰ�
	BOOL					ProcessRecvdPacket( DWORD dwMaxPacketSize );
	// �󶨾��
	void					BindNetworkObject( CNetworkObject *pNetworkObject );
	// �����
	void					UnbindNetworkObject();
	// ����Ϊ����
	void					OnAccept();
	// ��������
	void					OnConnect( BOOL bSuccess );
	void					OnLogString( char *pszLog, ... );
	// ��ַ��
	inline void				SetSocket( SOCKET socket ) {
		m_socket = socket;
	}
	// ��ַ����
	inline void	SetSockAddr( SOCKADDR_IN& sockaddr ) {
		m_sockaddr = sockaddr; }
	// �رյ�ַ
	inline void	CloseSocket() { 
		SocketOpt::CloseSocket( m_socket ); 
		m_socket = INVALID_SOCKET; 
	}
	// ����ӿ�
	inline CNetworkObject* GetNetworkObject() { 
		return m_pNetworkObject; 
	}
	// ���ͻ���
	inline CSendBuffer*	GetSendBuffer() { 
		return m_pSendBuffer; 
	}
	// ���ջ���
	inline CRecvBuffer*	GetRecvBuffer() { 
		return m_pRecvBuffer;
	} 
	// ������
	inline SOCKET GetSocket() { 
		return m_socket;
	}
	// �����ַ
	inline SOCKADDR_IN*	GetSockAddr() { 
		return &m_sockaddr; 
	}
	// ��ȡIP��ַ
	inline std::string GetIP() {
		return inet_ntoa( m_sockaddr.sin_addr ); 
	}
	// ��ȡ��ʱ 
	inline DWORD GetIdleTick() const { 
		return m_dwTimeOut? m_dwLastSyncTick + m_dwTimeOut : 0; 
	}
	// �Ƿ�ʱ
	inline BOOL	IsOnIdle() { 
		return m_dwTimeOut ? m_dwLastSyncTick + m_dwTimeOut < GetTickCount() : FALSE; 
	}
	// ��ȡ�±�
	inline DWORD GetIndex() { 
		return m_dwIndex; 
	}
	// �Ƿ�������
	inline BOOL IsAcceptSocket() { 
		return m_bAcceptSocket;
	}
	// ���ü��� 
	inline void SetAcceptSocketFlag() { 
		m_bAcceptSocket = TRUE; 
	}
	// �Ƴ�����
	void Remove() { 
		__sync_fetch_and_or (&m_bRemove, 1);
	}
	// ���ò���
	inline void ResetKillFlag() { 
		__sync_fetch_and_and (&m_bRemove, 0);
	}
	// �Ƿ��Ƴ�
	inline BOOL ShouldBeRemoved() { 
		return m_bRemove; 
	}
	// �Ͽ�����
	void Disconnect( BOOL bGracefulDisconnect );
	// �ж�����
	inline BOOL HasDisconnectOrdered() { 
		return m_bDisconnectOrdered; 
	}

private:
	// �����±�
	void SetIndex( DWORD index ) {
		m_dwIndex = index; 
	}
	// ����ʱ��
	inline void ResetTimeOut() { 
		m_dwLastSyncTick = GetTickCount(); 
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif
