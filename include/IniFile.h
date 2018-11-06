#ifndef _IniFile_Headfile_
#define _IniFile_Headfile_

/////////////////////////////////////////////////////////////////////////////////
#include <map>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////
// INI����

class CIniFile
{
	// ��������
public:
	// ���캯��
	CIniFile(const char * pIniFile);
	// ��������
	~CIniFile();

	// ���º���
public:
	// ��������
	void Update():
	// ��ȡ�ִ�
	const char * GetString(const char * sect, const char * key, const char * defaultValue = "");
	// �����ַ�
	void SetString(const char * sect, const char * key, const char * value;

	// ��ȡ����
    int GetInteger(const char* sect, const char* key, int defaultValue = 0);
	// ��������
	void SetInteger(const char* sect, const char* key, int value);

	// ��ȡ������
    double GetDouble(const char* sect, const char* key, double defaultValue = 0);
    // ���ø�����
	void SetDouble(const char* sect, const char* key, double value);
    
	// ��ȡ������
	bool GetBool(const char* sect, const char* key, bool defaultValue = false);
	// ���ò�����
	void SetBool(const char* sect, const char* key, bool value);
    
	// ɾ����ֵ
	void DeleteKey(const char* sect, const char* key);
    // ɾ����ֵ
	void DeleteSession(const char* sect);

	// �ļ��Ѿ���
	bool m_isOpenFile;

	// ��������
private: 
	// �ļ����ƺ�·��
    string									ini_file_name_; 
    // �ļ���ֵ
	map<string, string, less<string> >		ini_map_; 

private:
	// �ļ�����
	CIniFile(void) {
		m_isOpenFile = false; 
	};
	
	// ���ļ�
    bool OpenIni(const char* iniFile) ;
    // д���ļ�
	bool WriteIni(const char* iniFile) ;
}; 


/////////////////////////////////////////////////////////////////////////////////
#endif	// _IniFile_Headfile_
