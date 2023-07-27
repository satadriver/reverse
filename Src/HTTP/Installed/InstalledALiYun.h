#ifndef HTTP_INSTALLED_INSTALLEDALIYUN_H_H_H
#define HTTP_INSTALLED_INSTALLEDALIYUN_H_H_H

#include "Installed.h"

namespace installed
{
	class ALiYun : public Installed
	{
	public:
		ALiYun(HttpSession *http_session, const string &pro_name);
		~ALiYun();
		
		static int IsALiYun(HttpSession *http_session);
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