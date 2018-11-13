#ifndef _GameServerSession_Headfile_
#define _GameServerSession_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "ServerSession.h"

/////////////////////////////////////////////////////////////////////////////////
// ��Ϸ������

class CGameSession : public ServerSession
{
public:
	// ���캯��
	CGameSession(void);
	// ��������
	~CGameSession(void);
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

#endif