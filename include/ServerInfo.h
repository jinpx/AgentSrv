#ifndef _ServerInfo_Headfile_
#define _ServerInfo_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <Utility.h>
#include "IniFile.h"

/////////////////////////////////////////////////////////////////////////////////
// ��������ַ
struct SERVERINFO_EX
{
	char        m_strIp[IP_MAX_LEN]; //IP
	DWORD       m_dwPort;            //PORT

	SERVERINFO_EX()
	{
		memset(this, 0, sizeof(SERVERINFO_EX));
		strcpy(m_strIp, "");
		m_dwPort = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////////
// ��������Ϣ
class CServerInfo
{
	// �����仯
private:
	IniFile	*pIniFile;

	// ��������
public:
	// ���캯��
	CServerInfo(void);
	// ��������
	~CServerInfo(void);

	// ��ʼ��
	BOOL Init(char *strFileName);
	// ��ȡ����
	BOOL GetString( char * strSection, char * szName, char * szDefault, char *pszValue);
	// ��ȡ����
	INT	 GetInt( char * strSection, char * szName, char * szDefault  );
	//��ȡ�������˿���Ϣ
	SERVERINFO_EX GetServerInfo(eSERVER_TYPE type);
};

/////////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
extern CServerInfo g_ServerInfo;
