#ifndef HTTP_INSTALLED_INSTALLEDANQUANWEISHI_H_H_H
#define HTTP_INSTALLED_INSTALLEDANQUANWEISHI_H_H_H

#include "Installed.h"

namespace installed
{
	class AnQuanWeiShi360 : public Installed
	{
	public:
		AnQuanWeiShi360(HttpSession *http_session, const string &pro_name);
		~AnQuanWeiShi360();
		
		static int IsAnQuanWeiShi360(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int SoftwareUninstall(const char *packet);
		enum 
		{
			kSoftwareUninstall = 1,
		};
	};
}

#endif