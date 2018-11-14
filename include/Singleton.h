#ifndef _Singleton_Headfile_
#define _Singleton_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include "Utility.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////////
// Ψһʵ��

template <typename T>
class Singleton
{
	// ��������
private:
	// ʵ������
	static T *	ms_pInstance;

public:
	// ��������
	~Singleton()
	{ }
	// Ψһʵ��
	static T * Instance()
	{
		if (0 == ms_pInstance) {
			ms_pInstance = new T;
		}
		return ms_pInstance;
	}
	// ɾ��ʵ��
	static void DestroyInstance()
	{
		if (ms_pInstance) {		
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////
template< class T > T * Singleton<T>::ms_pInstance = 0;

/////////////////////////////////////////////////////////////////////////////////
#endif // __SINGLETON_H__
