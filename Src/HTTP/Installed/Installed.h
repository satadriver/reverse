
#ifndef HTTP_INSTALLED_INSTALLED_H_H_H
#define HTTP_INSTALLED_INSTALLED_H_H_H

#include "../json/json.h" //使用json 提取字符串
#include "../HttpPro.h"

namespace installed
{
	class Installed : public HttpPro
	{
	public:
		Installed(HttpSession *http_session, const string &pro_name);
		//虚析构函数	当一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用
		virtual ~Installed();
		//静态成员函数  不需要任何对象实例即可访问
		static int ProcessInstalled(HttpSession *http_session ,const char *packet);
		static void InitApkAppMap();
		static void InitUselessAPK();
	protected:
		string GetAppName(const string &apk);
		string installed_;
		string app_;
		//apk,app_name
		//静态成员
		static map<string,string>apk_app_;
		static set<string>useless_apk_;
	private:
		static void StringReplace(string &src_string,const string &old_string ,const string &new_string);	
		//static int app_total_;			
	};
}

#endif