#ifndef _Network_Headfile_
#define _Network_Headfile_

#pragma pack(push,1)
/////////////////////////////////////////////////////////////////////////////////
// ��ͷ

typedef struct tagPACKET_HEADER
{
	WORD		size;			// ���ݰ���С
} PACKET_HEADER;


/////////////////////////////////////////////////////////////////////////////////
// �������

class NetworkObject
{
	// �Ự����
	friend class Session;
	// ���ƾ��
	friend class IoHandler;
	
	// ��������
private:
	Session *					m_pSession;			// �Ự����

public:
	// ���캯��
	NetworkObject();
	// ��������
	virtual ~NetworkObject();
	// �ر�����
	void Disconnect( BOOL bGracefulDisconnect = TRUE );
	// ��������
	BOOL Send( BYTE *pMsg, WORD wSize );
	// ��������
	BOOL SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize );
	// �󶨲���
	void Redirect( NetworkObject *pNetworkObject );
	// ��ȡ��ַ
	SOCKADDR_IN* GetSockAddr();
	// ��ȡ��ַ
	std::string GetIP();

	// �������
private:
	// ��������
	virtual void OnAccept( DWORD dwNetworkIndex ) {}
	// �ر�����
	virtual void OnDisconnect() {}
	// ��������
	virtual	void OnRecv( BYTE *pMsg, WORD wSize ) = 0;
	// ���Ӳ���
	virtual void OnConnect( BOOL bSuccess, DWORD dwNetworkIndex ) {}
	// ��ӡ��־
	virtual void OnLogString( char *pszLog ) {}
	// ���ûỰ
	inline void	SetSession( Session *pSession ) { 
		m_pSession = pSession; 
	}
};


/////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif
