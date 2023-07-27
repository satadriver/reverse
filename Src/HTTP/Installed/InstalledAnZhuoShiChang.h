#ifndef HTTP_INSTALLED_INSTALLEDANZHUOSHICHANG_H_H_H
#define HTTP_INSTALLED_INSTALLEDANZHUOSHICHANG_H_H_H
/************************************************************************/
/* 安卓市场                         
在安装应用的出现 包                                   */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class AnZhuoShiChang : public Installed
	{
	public:
		AnZhuoShiChang(HttpSession *http_session, const string &pro_name);
		~AnZhuoShiChang();
		
		static int IsAnZhuoShiChang(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		int SoftwareUpdate1(const char *packet); 
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif