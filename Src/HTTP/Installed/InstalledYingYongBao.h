#ifndef HTTP_INSTALLED_INSTALLEDYINGYONGBAO_H_H_H
#define HTTP_INSTALLED_INSTALLEDYINGYONGBAO_H_H_H
#include "Installed.h"

//暂时未处理 。。。。。
namespace installed
{
	class YingYongBao : public Installed
	{
	public:
		YingYongBao(HttpSession *http_session, const string &pro_name);
		~YingYongBao();
		
		static int IsYingYongBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		//不解压压缩包
		int SoftwareUpdate1(const char *packet); 
		//解压压缩包
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif