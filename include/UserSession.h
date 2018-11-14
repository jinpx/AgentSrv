#ifndef _UserSession_Headfile_
#define _UserSession_Headfile_

/////////////////////////////////////////////////////////////////////////////////
enum eUSER_TYPE { UT_TEMP_USER, UT_USER, };

/////////////////////////////////////////////////////////////////////////////////
// �û��Ự

class CUserSession : public CServerSession
{
	// ��������
protected:
	eUSER_TYPE				m_eUserType;						// �û�����
	DWORD					m_dwUserId;							// �û���ʶ
	TCHAR					m_pszID[MAX_ID_LEN+1];				// �ʻ�����
	DWORD					m_dwUserKey;						// �û���ֵ
	TCHAR					m_pszLoginTime[MAX_TIMEDATA_SIZE];	// ��¼ʱ��
	ServerSession *			m_pGameServer;

	// ��������
public:
	// ���캯��
	CUserSession(void);
	// ��������
	~CUserSession(void);

	// �û�����
public:
	// �û���ʶ
	DWORD GetUserID() { 
		return m_dwUserId; 
	}
	// �û���ʶ
	VOID SetUserID( DWORD dwUserID ) {
		m_dwUserId = dwUserID;
	}
	// �����û�����
	VOID SetUsername( const TCHAR * ps );	
	// ��ȡ�û�����
	const TCHAR * GetUsername() { 
		return m_pszID;
	}
	// �û���ֵ
	VOID SetUserKey( DWORD dwKey ) { 
		m_dwUserKey = dwKey; 
	}
	// �û���ֵ
	DWORD GetUserKey() const { 
		return m_dwUserKey; 
	}
	// ����IP����
	VOID SetClientIP( const TCHAR * ps ) {
		strcpy(m_pszClientIP, ps);
	}
	// ��¼ʱ��
	VOID SetLoginTime( const TCHAR * ps ) { 
		strncpy( m_pszLoginTime, ps, MAX_TIMEDATA_SIZE ); 
		m_pszID[MAX_ID_LEN-1]=0; 
	}
	// ��ȡʱ��
	const TCHAR * GetLoginTime() { 
		return m_pszLoginTime; 
	}
	// ��Ϸ������
	VOID UnGameServer();

	// ���ຯ��
public:
	// ��������
	virtual BOOL SendPacket( MSG_BASE * pMsg, WORD wSize ); 
	// ��������
	virtual VOID OnRecv( BYTE * pMsg, WORD wSize );
	// ��Ϸ������
	ServerSession *	GetGameServer() {
		return m_pGameServer;
	}
	// ��ȡ����
	eSERVER_TYPE GetServerType() {
		return GAME_SERVER;
	}
	//�û�����
	virtual eUSER_TYPE	getUserType() { 
		return m_eUserType; 
	}
	// �û�����
	virtual VOID setUserType( eUSER_TYPE type ) {
		m_eUserType = type; 
	}

protected:
	// ��ӡ��־
	virtual VOID OnLogString( char *pszLog );
	// ��ʼ��
	virtual VOID Init();
	// �ͷŲ���
	virtual VOID release();
	// ��������
	virtual VOID OnDisconnect();

};

/////////////////////////////////////////////////////////////////////////////////
#endif
