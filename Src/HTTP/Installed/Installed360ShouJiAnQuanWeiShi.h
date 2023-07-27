#ifndef HTTP_INSTALLED_INSTALLED360SHOUJIANQUANWEISHI_H_H_H
#define HTTP_INSTALLED_INSTALLED360SHOUJIANQUANWEISHI_H_H_H

#include "Installed.h"

namespace installed
{
	class ShouJiAnQuanWeiShi360 : public Installed
	{
	public:
		ShouJiAnQuanWeiShi360(HttpSession *http_session, const string &pro_name);
		~ShouJiAnQuanWeiShi360();
		
		static int IsShouJiAnQuanWeiShi360(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int SoftwareUpdate(const char *packet);
		enum 
		{
			kSoftwareUpdate = 1,
		};
	};
}

#endif