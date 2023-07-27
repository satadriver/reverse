#ifndef HTTP_INSTALLED_INSTALLEDBAIDUSHOUJIZHUSHOU_H_H_H
#define HTTP_INSTALLED_INSTALLEDBAIDUSHOUJIZHUSHOU_H_H_H
/************************************************************************/
/* 百度手机助手 和 安卓市场是一样的                         
在安装应用的出现 包                                   */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class BaiDuShouJiZhuShou : public Installed
	{
	public:
		BaiDuShouJiZhuShou(HttpSession *http_session, const string &pro_name);
		~BaiDuShouJiZhuShou();
		
		static int IsBaiDuShouJiZhuShou(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif