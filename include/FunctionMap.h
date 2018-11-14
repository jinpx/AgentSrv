#ifndef _FunctionMap_Headfile_
#define _FunctionMap_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "Utility.h"
#include <ext/hash_map>
using  namespace   __gnu_cxx;
#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))

/////////////////////////////////////////////////////////////////////////////////
// 函数基类

class BASE_FUNC
{
public:
	// 构造函数
	BASE_FUNC(){}
	// 析构函数
	virtual ~BASE_FUNC(){}
	// 函数键值
	DWORD m_dwFunctionKey;
};

/////////////////////////////////////////////////////////////////////////////////
// 函数映射

class CFunctionMap
{
	typedef hash_map< DWORD, BASE_FUNC * >			MAP_FUNCTION;
	typedef MAP_FUNCTION::iterator					MAP_FUNCTION_IT;
	typedef MAP_FUNCTION::value_type				MAP_FUNCTION_VT;

	// 参数变量
protected:
	MAP_FUNCTION					m_mFunction;					//
	MAP_FUNCTION_IT					m_itr;

	// 基本函数
public:
	// 构造函数
	CFunctionMap();
	// 析构函数
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
	// 参数开始
	VOID SetFirst() {
		m_itr = m_mFunction.begin();
	}
	// 下个函数
	BOOL GetNext( BASE_FUNC *& pFuncOut )
	{
		++m_itr;
		pFuncOut = m_itr->second;
		if(m_itr != m_mFunction.end()) {
			return TRUE;
		}
		return FALSE;
	}
	// 添加函数
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
