#include <Utility.h>
#include "PacketHandler.h"
#include "ServerSession.h"
#include "UserSession.h"

/////////////////////////////////////////////////////////////////////////////////
CPacketHandler g_PacketHander;

/////////////////////////////////////////////////////////////////////////////////
CPacketHandler::CPacketHandler(void)
{
	m_pFuncMap_CA = new CFunctionMap;
	m_pFuncMap_AL = new CFunctionMap;
	m_pFuncMap_AG = new CFunctionMap;
	RegisterHandler();
}

/////////////////////////////////////////////////////////////////////////////////
CPacketHandler::~CPacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_CA);
	SAFE_DELETE(m_pFuncMap_AL);
	SAFE_DELETE(m_pFuncMap_AG);
}

/////////////////////////////////////////////////////////////////////////////////
//注册所有消息
BOOL CPacketHandler::RegisterHandler()
{
	Register_CA();
	Register_AD();
	Register_AM();
	return TRUE;
}	

/////////////////////////////////////////////////////////////////////////////////
//需要转发到GameServer的消息
void CPacketHandler::AddSendGameSrvMsg(WORD category, WORD protocol)
{
	m_mapSendGameSrvMsg[MAKELONG(category, protocol)] = 1;
}

/////////////////////////////////////////////////////////////////////////////////
void CPacketHandler::Register_CA()
{
	AddHandler_CA(CG_SYNC, CG_SYNC_PLAYER_ENTER_SYN,  Handler_FromClient::OnCG_SYNC_PLAYER_ENTER_SYN);
}

/////////////////////////////////////////////////////////////////////////////////
void CPacketHandler::Register_AL()
{
	AddHandler_AD(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_ACK, Hander_FromDBSrv::OnAD_CHARINFO_CHARLISTREQ_ACK);
}

/////////////////////////////////////////////////////////////////////////////////
void CPacketHandler::Register_AG()
{
}


/////////////////////////////////////////////////////////////////////////////////
BOOL CPacketHandler::AddHandler_CA( WORD category, WORD protocol, fnHandler_c fnHandler)
{
	FUNC_CA * pFuncInfo	= new FUNC_CA;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_CA->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
BOOL CPacketHandler::AddHandler_AL( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_AL * pFuncInfo	= new FUNC_AL;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_AL->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
BOOL CPacketHandler::AddHandler_AG( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_AG * pFuncInfo	= new FUNC_AG;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_AG->Add( pFuncInfo );

}

/////////////////////////////////////////////////////////////////////////////////
//解析消息
VOID CPacketHandler::ParsePacket_CA( UserSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	ASSERT(NULL != pMsg);
	
	//如果是聊天服务器消息就转发
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

	//如果是农场服务器的消息，则直接转发
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
VOID CPacketHandler::ParsePacket_AL( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	FUNC_AL * pFuncInfo = (FUNC_AL *)m_pFuncMap_AD->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if(pFuncInfo == NULL) {
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

/////////////////////////////////////////////////////////////////////////////////
VOID CPacketHandler::ParsePacket_AG( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	FUNC_AG * pFuncInfo = (FUNC_AG *)m_pFuncMap_AM->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if(pFuncInfo == NULL) {
		return;
	}
	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

