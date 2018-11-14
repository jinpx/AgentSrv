#ifndef _PacketHandler_Headfile_
#define _PacketHandler_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <PacketStruct.h>
#include <Singleton.h>
#include <map>

/////////////////////////////////////////////////////////////////////////////////
class CServerSession;
class CUserSession;

/////////////////////////////////////////////////////////////////////////////////
// �������
typedef VOID (*fnHandler)( CServerSession * pSession, MSG_BASE * pMsg, WORD wSize );
typedef VOID (*fnHandler_c)( CUserSession * pSession, MSG_BASE * pMsg, WORD wSize );


/////////////////////////////////////////////////////////////////////////////////
// ���ݰ�����
class CPacketHandler: public Singleton<CPacketHandler>
{
public:
	// ���캯��
	CPacketHandler(void);
	// ��������
	~CPacketHandler(void);

public:
	// ע����Ϣ
	BOOL RegisterHandler();

	// ע��ͻ���������Ϣ
	void Register_CA();
	// ע���¼��Ϣ
	void Register_AL();
	// ע����Ϸ��Ϣ
	void Register_AG();

	// ��ӷ�����Ϣ
	void AddSendGameSrvMsg(WORD category, WORD protocol);

	// ��Ӵ�����Ϣ���
	BOOL AddHandler_CA( WORD category, WORD protocol, fnHandler_c fnHandler);
	// ��Ӵ�����Ϣ���
	BOOL AddHandler_AL( WORD category, WORD protocol, fnHandler fnHandler);
	// ��Ӵ�����Ϣ���
	BOOL AddHandler_AG( WORD category, WORD protocol, fnHandler fnHandler);

	//������Ϣ
	VOID ParsePacket_CA( UserSession  * pSession, MSG_BASE * pMsg, WORD wSize );
	//������Ϣ
	VOID ParsePacket_AL( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize );
	//������Ϣ
	VOID ParsePacket_AG( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize );

private:

	//�ͻ���<-->���ط���������Ϣ
	struct FUNC_CA : public BASE_FUNC {
		fnHandler_c	m_fnHandler;
	};

	//���ط�����<-->���ݿ����������Ϣ
	struct FUNC_AL : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	//���ط�����<-->��ط���������Ϣ
	struct FUNC_AG : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

private:
	//�ͻ���<-->���ط���������Ϣ
	FunctionMap	*	m_pFuncMap_CA;

	//���ط�����<-->���ݿ����������Ϣ
	FunctionMap	*	m_pFuncMap_AL;

	//���ط�����<-->��ط���������Ϣ
	FunctionMap	*	m_pFuncMap_AG;

	//��Ҫת������Ϸ����������Ϣ
	std::map<DWORD, BYTE>	m_mapSendGameSrvMsg;
};

/////////////////////////////////////////////////////////////////////////////////
// ȫ�ְ�����
extern CPacketHandler g_PacketHander;

/////////////////////////////////////////////////////////////////////////////////
#endif
