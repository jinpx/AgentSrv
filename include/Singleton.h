#ifndef _Singleton_Headfile_
#define _Singleton_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "Utility.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////////
// 唯一实例

template <typename T>
class CSingleton
{
	// 参数变量
private:
	// 实例参数
	static T *	ms_pInstance;

public:
	// 析构函数
	~CSingleton()
	{ }
	// 唯一实例
	static T * Instance()
	{
		if (0 == ms_pInstance) {
			ms_pInstance = new T;
		}
		return ms_pInstance;
	}
	// 删除实例
	static void DestroyInstance()
	{
		if (ms_pInstance) {		
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////
template< class T > T * CSingleton<T>::ms_pInstance = 0;

/////////////////////////////////////////////////////////////////////////////////
#endif // __SINGLETON_H__
