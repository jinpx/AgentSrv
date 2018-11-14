#ifndef _FunctionMap_Headfile_
#define _FunctionMap_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "Utility.h"
#include <ext/hash_map>
using  namespace   __gnu_cxx;
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))

/////////////////////////////////////////////////////////////////////////////////
// ��������

class BASE_FUNC
{
public:
	// ���캯��
	BASE_FUNC(){}
	// ��������
	virtual ~BASE_FUNC(){}
	// ������ֵ
	DWORD m_dwFunctionKey;
};

/////////////////////////////////////////////////////////////////////////////////
// ����ӳ��

class CFunctionMap
{
	typedef hash_map< DWORD, BASE_FUNC * >			MAP_FUNCTION;
	typedef MAP_FUNCTION::iterator					MAP_FUNCTION_IT;
	typedef MAP_FUNCTION::value_type				MAP_FUNCTION_VT;

	// ��������
protected:
	MAP_FUNCTION					m_mFunction;					//
	MAP_FUNCTION_IT					m_itr;

	// ��������
public:
	// ���캯��
	CFunctionMap();
	// ��������
	~CFunctionMap()
	{
		BASE_FUNC * pFunc;
		MAP_FUNCTION_IT itr;
		MAP_FUNCTION_IT itr_bk;
		for(itr = m_mFunction.begin(); itr != m_mFunction.end();)
		{
			pFunc = itr->second;
			if(pFunc) {
				delete pFunc;
				pFunc = NULL;
			}

			itr_bk = itr;
			itr++;
			m_mFunction.erase(itr_bk);
		}
		m_mFunction.clear();
	}
	// ������ʼ
	VOID SetFirst() {
		m_itr = m_mFunction.begin();
	}
	// �¸�����
	BOOL GetNext( BASE_FUNC *& pFuncOut )
	{
		++m_itr;
		pFuncOut = m_itr->second;
		if(m_itr != m_mFunction.end()) {
			return TRUE;
		}
		return FALSE;
	}
	// ��Ӻ���
	BOOL  Add( BASE_FUNC * pFunction )
	{
		MAP_FUNCTION_IT			it;
		BASE_FUNC *				pTemp = NULL;

		if( !m_mFunction.empty() ) {
			it = m_mFunction.find( pFunction->m_dwFunctionKey );
			if(it != m_mFunction.end()) {
				pTemp = it->second;
			}
		}

		if( pTemp != NULL )  {
			return FALSE;
		}

		m_mFunction.insert(MAP_FUNCTION_VT( pFunction->m_dwFunctionKey, pFunction ));
		return TRUE;
	}

	BASE_FUNC * Find(const DWORD dwFunctionKey)
	{
		MAP_FUNCTION_IT			it;
		BASE_FUNC *				pTemp = NULL;

		if( !m_mFunction.empty() ) {
			it = m_mFunction.find( dwFunctionKey );
			if(it != m_mFunction.end())	{
				pTemp = it->second;
			}
		}

		return pTemp;
	}
};

/////////////////////////////////////////////////////////////////////////////////
#endif // __FUNCTION_MAP_H__
