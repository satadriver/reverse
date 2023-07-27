
#ifndef HTTP_INSTALLED_INSTALLED_H_H_H
#define HTTP_INSTALLED_INSTALLED_H_H_H

#include "../json/json.h" //ʹ��json ��ȡ�ַ���
#include "../HttpPro.h"

namespace installed
{
	class Installed : public HttpPro
	{
	public:
		Installed(HttpSession *http_session, const string &pro_name);
		//����������	��һ�������ָ��ɾ��һ��������Ķ���ʱ������������������ᱻ����
		virtual ~Installed();
		//��̬��Ա����  ����Ҫ�κζ���ʵ�����ɷ���
		static int ProcessInstalled(HttpSession *http_session ,const char *packet);
		static void InitApkAppMap();
		static void InitUselessAPK();
	protected:
		string GetAppName(const string &apk);
		string installed_;
		string app_;
		//apk,app_name
		//��̬��Ա
		static map<string,string>apk_app_;
		static set<string>useless_apk_;
	private:
		static void StringReplace(string &src_string,const string &old_string ,const string &new_string);	
		//static int app_total_;			
	};
}

#endif