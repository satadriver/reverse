#ifndef HTTP_INSTALLED_INSTALLED91ZHUSHOU_H_H_H
#define HTTP_INSTALLED_INSTALLED91ZHUSHOU_H_H_H

#include "Installed.h"

namespace installed
{
	class ZhuShou91 : public Installed
	{
	public:
		ZhuShou91(HttpSession *http_session, const string &pro_name);
		~ZhuShou91();
		
		static int IsZhuShou91(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int SoftwareUpdate(const char *packet);
		enum 
		{
			kSoftwareUpdate = 1
		};
	};
}

#endif