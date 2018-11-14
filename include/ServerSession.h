#ifndef _ServerSeesion_Headfile_
#define _ServerSeesion_Headfile_

#pragma once
/////////////////////////////////////////////////////////////////////////////////
#include <Network.h>

/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// ����������

enum eSERVER_TYPE
{
	UNKNOWN_SERVER 			= 0,
	TEMP_SERVER				,
	LOGON_SERVER			,		//��¼������
	GAME_SERVER				,		//��Ϸ������
	DB_SERVER				,		//���ݿ����
};

/////////////////////////////////////////////////////////////////////////////////
// �������Ự����

class CServerSession : public CNetworkObject
{
	// ��������Ԫ
	// friend class CAgentSrv;
	
	// ��������
private:
	DWORD					m_dwSessionIndex;
	std::string				m_strConnectIP;
	WORD					m_wConnectPort;
	BOOL					m_bForConnect;
	DWORD					m_dwLastHeartbeatTick;
	DWORD					m_bConnection;

public:
	// ���캯��
	CServerSession();
	// ��������
	~CServerSession();

	// ��������
	virtual VOID Clear();
	// ��ʼ����
	virtual VOID Init();
	// �ͷ�����
	virtual VOID Release();
	// ��������
	virtual VOID Update();
	// ��������
	virtual VOID OnAccept( DWORD dwNetworkIndex );
	// �Ͽ�����
	virtual VOID OnDisconnect();
	// ��������
	virtual VOID OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	// ��������
	virtual VOID OnRecv( BYTE *pMsg, WORD wSize );
	// ��ӡ��־
	virtual VOID OnLogString( char *pszLog );
	// ��������
	virtual eSERVER_TYPE GetServerType() { 
		return UNKNOWN_SERVER;
	}
	// �Ự��ʶ
	DWORD GetSessionIndex() { 
		return m_dwSessionIndex; 
	}
	// ���ñ�ʶ
	VOID SetSessionIndex( DWORD dwIdx ) { 
		m_dwSessionIndex = dwIdx; 
	}
	// ��������
	VOID SendServerType();
	// ���õ�ַ
	VOID SetAddr( char *pszIP, WORD wPort );
	// ��������
	VOID TryToConnect();
	// ����IP
	inline std::string & GetConnectIP() { 
		return m_strConnectIP; 
	}
	// ���Ӷ˿�
	inline WORD GetConnectPort() { 
		return m_wConnectPort; 
	}
	// ��������
	inline BOOL IsConnected() { 
		return m_bConnection; 
	}
	// ��������
	inline VOID SetForConnect( BOOL bForConnect ) { 
		m_bForConnect = bForConnect; 
	}
	// �ж�����
	inline BOOL IsForConnect() { 
		return m_bForConnect;
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // __SERVER_SESSION_H__
