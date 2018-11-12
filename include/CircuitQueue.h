#ifndef _CircuitQueue_Headfile_
#define _CircuitQueue_Headfile_

#include <stdio.h>
#include <assert.h>
#include <CircuitLock.h>

/////////////////////////////////////////////////////////////////////////////////
// 循环队列

template<typename T>
class CircuitQueue
{
	//变量与参数
protected:
	CCircuitLock		m_cs;				
	T					*m_pData;									// 循环锁
	int					m_nLength;									// 数据长度
	int					m_nSize;									// 数据大小
	int					m_nHead;									// 头指钟
	int					m_nTail;									// 尾指钟 
	int					m_nExtraSize;								// 外数据

	// 基本函数
public:
	// 构造函数
	CircuitQueue()
		: m_pData(NULL)
		, m_nLength(0)
		, m_nSize(0)
		, m_nHead(0)
		, m_nTail(0)
	{ }

	// 析构函数
	virtual ~CircuitQueue() {
		if (m_pData) {
			delete[] m_pData;
		}
	}

	// 最新函数
public:
	// 创建数据
	void Create( int nSize, int nExtraSize = 0)
	{
		m_cs.Lock();
		if( m_pData ) {
			delete [] m_pData;
		}

		m_pData			= new T[nSize + nExtraSize];
		m_nSize			= nSize;
		m_nExtraSize	= nExtraSize;
		m_cs.Unlock();
	}
	// 清理数据
	inline void Clear()
	{
		m_cs.Lock();

		m_nLength       = 0;
		m_nHead         = 0;
		m_nTail         = 0;
		m_cs.Unlock();
	}
	// 取得空间大小
	inline int GetSpace()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nSize - m_nLength;		
		m_cs.Unlock();
		return iRet;
	}
	// 数据长度
	inline int GetLength()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nLength;
		m_cs.Unlock();
		return iRet;
	}
	// 取得块数据长度
	inline int GetBackDataCount()
	{
		int iRet;

		m_cs.Lock();
		iRet = m_nSize - m_nHead;
		m_cs.Unlock();
		return iRet;
	}
	// 读取指针
	inline T* GetReadPtr()
	{
		T *pRet;

		m_cs.Lock();
		pRet = m_pData + m_nHead;

		int nSplitFirstDataCount;
		if( m_nHead > m_nTail && ( nSplitFirstDataCount = m_nSize - m_nHead ) < m_nExtraSize ) {
			memcpy( m_pData + m_nSize, m_pData, sizeof(T) * ( m_nExtraSize - nSplitFirstDataCount ) );
		}

		m_cs.Unlock();
		return pRet;
	}
	// 写数据
	inline T* GetWritePtr()
	{
		T *pRet;

		m_cs.Lock();
		pRet = m_pData + m_nTail;
		m_cs.Unlock();
		return pRet;
	}
	// 可读取长度
	inline int GetReadableLen()
	{
		int iRet;

		m_cs.Lock();
		if( m_nHead == m_nTail ) {
			iRet = GetLength() > 0 ? m_nSize - m_nHead: 0;
		}
		else if( m_nHead < m_nTail ) {
			iRet = m_nTail - m_nHead;
		}
		else {
			iRet = m_nSize - m_nHead;
		}
		m_cs.Unlock();
		return iRet;
	}
	// 可写入长度
	inline int GetWritableLen()
	{
		int iRet;

		m_cs.Lock();
		if( m_nHead == m_nTail ) {
			iRet = GetLength() > 0 ? 0 : m_nSize - m_nTail;
		}
		else if( m_nHead < m_nTail ) {
			iRet = m_nSize - m_nTail;
		}
		else {
			iRet = m_nHead - m_nTail;
		}
		m_cs.Unlock();
		return iRet;
	}
	// 提出队列
	inline BOOL Enqueue( T *pSrc, int nSize )
	{
		// 判断大小
		m_cs.Lock();
		if( GetSpace() < nSize ) {
			m_cs.Unlock();
			return FALSE;
		}

		// 提取参数
		if( pSrc )
		{
			if( m_nHead <= m_nTail )
			{
				int nBackSpaceCount = m_nSize - m_nTail;
				if( nBackSpaceCount >= nSize ) {
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
				}
				else {
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount );
					memcpy( m_pData, pSrc + nBackSpaceCount, sizeof(T) * ( nSize - nBackSpaceCount ) );
				}
			}
			else {
				memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
			}
		}

		m_nTail		+= nSize;
		m_nTail		%= m_nSize;
		m_nLength	+= nSize;
		m_cs.Unlock();
		return TRUE;
	}
	// 压入队列
	inline BOOL Dequeue( T *pTar, int nSize )
	{    
		m_cs.Lock();

		if( !Peek( pTar, nSize ) ) {
			m_cs.Unlock();
			return FALSE;
		}

		m_nHead		+= nSize;
		m_nHead		%= m_nSize;
		m_nLength	-= nSize;
		m_cs.Unlock();
		return TRUE;
	}
	// 压入队列算法
	inline BOOL Peek( T *pTar, int nSize )
	{
		m_cs.Lock();
		if( m_nLength < nSize ) {
			m_cs.Unlock();
			return FALSE;
		}

		//  压入数据
		if( pTar!=NULL )
		{
			if( m_nHead<m_nTail ) {
				memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );
			}
			else {
				if( GetBackDataCount() >= nSize ) {
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );                           
				}
				else {
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * GetBackDataCount() );
					memcpy( pTar + GetBackDataCount(), m_pData, sizeof(T)*(nSize-GetBackDataCount()));
				}
			}
		}

		m_cs.Unlock();
		return TRUE;
	}
	// 拷贝数据
	inline void CopyHeadDataToExtraBuffer( int nSize )
	{
		assert( nSize<=m_nExtraSize );
		m_cs.Lock();

		// 拷贝数据
		memcpy( m_pData+m_nSize, m_pData, nSize );
		m_cs.Unlock();
	}
};

#endif