#ifndef HTTP_INSTALLED_INSTALLEDXIAOMISHANGDIAN_H_H_H
#define HTTP_INSTALLED_INSTALLEDXIAOMISHANGDIAN_H_H_H

#include "Installed.h"

namespace installed
{
	class XiaoMiShangDian : public Installed
	{
	public:
		XiaoMiShangDian(HttpSession *http_session, const string &pro_name);
		~XiaoMiShangDian();
		
		static int IsXiaoMiShangDian(HttpSession *http_session);
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