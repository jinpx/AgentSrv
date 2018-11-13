#ifndef _LogonSession_Headfile_
#define _LogonSession_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "ServerSession.h"

/////////////////////////////////////////////////////////////////////////////////
// ��Ϸ������
class CLogonSession:public ServerSession
{
public:
	// ���캯��
	CLogonSession(void);
	// ��������
	~CLogonSession(void);
	// ��������
	virtual VOID OnRecv( BYTE * pMsg, WORD wSize );
	// ��ȡ����
	eSERVER_TYPE GetServerType() {
		return GAME_SERVER;
	}

protected:
	// ��������
	virtual VOID OnDisconnect();
	// ��ӡ��־
	virtual VOID OnLogString( char *pszLog );
	// ��ʼ��
	virtual VOID Init();
};

/////////////////////////////////////////////////////////////////////////////////
#endif
