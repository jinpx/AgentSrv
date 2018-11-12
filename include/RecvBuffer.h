#ifndef _RecvBuffer_Headfile_
#define _RecvBuffer_Headfile_

#include "CircuitQueue.h"
/////////////////////////////////////////////////////////////////////////////////

//接收缓存
class RecvBuffer
{
	//变量数据
private:
	CircuitQueue<BYTE>				*m_pQueue;						// 参数队列

public:
	// 构造函数
	RecvBuffer() { 
		m_pQueue = NULL;
	}
	// 析构函数
	virtual ~RecvBuffer() {
		if( m_pQueue ) {
			delete m_pQueue; 
		}
	}
	// 创建数据块
	inline void	Create( int nBufferSize, DWORD dwExtraBufferSize ) {
		if( m_pQueue ) {
			delete m_pQueue;
		}
		m_pQueue = new CircuitQueue<BYTE>;
		m_pQueue->Create( nBufferSize, dwExtraBufferSize ); 
	}
	// 数据完成
	inline void	Completion( int nBytesRecvd ) {
		m_pQueue->Enqueue( NULL, nBytesRecvd ); 
	}
	// 获取指针
	inline void	Clear() { 
		m_pQueue->Clear(); 
	}
	// 获取参数
	inline void	GetRecvParam( BYTE **ppRecvPtr, int &nLength ) {
		*ppRecvPtr	= m_pQueue->GetWritePtr();		
		nLength		= m_pQueue->GetWritableLen(); 
	}
	// 数据指针
	inline BYTE* GetFirstPacketPtr() {
		PACKET_HEADER   header;
		int nPktSize = sizeof(PACKET_HEADER);

		// 提取数据
		if(!m_pQueue->Peek( (BYTE*)&header, nPktSize )) {
			return NULL;
		}

		// 再次提示
		nPktSize += header.size;
		if(m_pQueue->GetLength() < nPktSize) {
			return NULL;
		}
		
		// 数据复制
		if(m_pQueue->GetBackDataCount() < nPktSize) {
			m_pQueue->CopyHeadDataToExtraBuffer( nPktSize - m_pQueue->GetBackDataCount() ); 
		}
		return  m_pQueue->GetReadPtr();
	}
	// 移除数据包
	inline void	RemoveFirstPacket( WORD wSize ) { 
		m_pQueue->Dequeue( NULL, wSize ); 
	}
};

#endif
