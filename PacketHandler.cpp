#include <Define.h>
#include "PacketHandler.h"
#include "ServerSession.h"
#include "UserSession.h"

/////////////////////////////////////////////////////////////////////////////////
PacketHandler g_PacketHander;

/////////////////////////////////////////////////////////////////////////////////
PacketHandler::PacketHandler(void)
{
	m_pFuncMap_CA = new FunctionMap;
	m_pFuncMap_AL = new FunctionMap;
	m_pFuncMap_AG = new FunctionMap;
	RegisterHandler();
}

/////////////////////////////////////////////////////////////////////////////////
PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_CA);
	SAFE_DELETE(m_pFuncMap_AL);
	SAFE_DELETE(m_pFuncMap_AG);
}

/////////////////////////////////////////////////////////////////////////////////
//ע��������Ϣ
BOOL PacketHandler::RegisterHandler()
{
	Register_CA();
	Register_AD();
	Register_AM();
	return TRUE;
}	

/////////////////////////////////////////////////////////////////////////////////
//��Ҫת����GameServer����Ϣ
void PacketHandler::AddSendGameSrvMsg(WORD category, WORD protocol)
{
	m_mapSendGameSrvMsg[MAKELONG(category, protocol)] = 1;
}

/////////////////////////////////////////////////////////////////////////////////
void PacketHandler::Register_CA()
{
	AddHandler_CA(CG_SYNC, CG_SYNC_PLAYER_ENTER_SYN,  Handler_FromClient::OnCG_SYNC_PLAYER_ENTER_SYN);
}

/////////////////////////////////////////////////////////////////////////////////
void PacketHandler::Register_AL()
{
	AddHandler_AD(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_ACK, Hander_FromDBSrv::OnAD_CHARINFO_CHARLISTREQ_ACK);
}

/////////////////////////////////////////////////////////////////////////////////
void PacketHandler::Register_AG()
{
}


/////////////////////////////////////////////////////////////////////////////////
BOOL PacketHandler::AddHandler_CA( WORD category, WORD protocol, fnHandler_c fnHandler)
{
	FUNC_CA * pFuncInfo	= new FUNC_CA;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_CA->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
BOOL PacketHandler::AddHandler_AL( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_AL * pFuncInfo	= new FUNC_AL;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_AL->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
BOOL PacketHandler::AddHandler_AG( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_AG * pFuncInfo	= new FUNC_AG;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_AG->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
//������Ϣ
VOID PacketHandler::ParsePacket_CA( UserSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	ASSERT(NULL != pMsg);
	
	//����������������Ϣ��ת��
	if ( pMsg->m_byCategory==CW_SYNC )
	{ 
		if (pMsg->m_byProtocol==CW_SYNC_FRIENDS_INFO_REQ )
		{
			MSG_CW_FRIENDS_INFO_SYNC* rMsg = NULL;
			rMsg = (MSG_CW_FRIENDS_INFO_SYNC*)pMsg;
			
			User *pUser = (User *)pSession;
			if ( pUser==NULL ) return;
			rMsg->dwUserGUID = pUser->GetUserGUID();
		}

		g_pAgentServer->SendToWorldServer( (BYTE*)pMsg, wSize );
		return;
	}

	//�����ũ������������Ϣ����ֱ��ת��
	if (AF_CONNECTION <= pMsg->m_byCategory && AF_MAXCMD > pMsg->m_byCategory)
	{
		if (NULL != g_pAgentServer)
		{
			((MSG_BASE_FORWARD *)pMsg)->m_dwParameter = pSession->GetUserGUID();
			((MSG_BASE_FORWARD *)pMsg)->m_byParameter = pSession->GetSelectedCharIndex();
			g_pAgentServer->SendToFarmServer((BYTE*)pMsg, wSize);
		}
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////
VOID PacketHandler::ParsePacket_AL( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	FUNC_AL * pFuncInfo = (FUNC_AL *)m_pFuncMap_AD->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if(pFuncInfo == NULL) {
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

/////////////////////////////////////////////////////////////////////////////////
VOID PacketHandler::ParsePacket_AG( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	FUNC_AG * pFuncInfo = (FUNC_AG *)m_pFuncMap_AM->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if(pFuncInfo == NULL) {
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}
