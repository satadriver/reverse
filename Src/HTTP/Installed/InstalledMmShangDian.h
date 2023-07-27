#ifndef HTTP_INSTALLED_INSTALLEDMMSHANGDIAN_H_H_H
#define HTTP_INSTALLED_INSTALLEDMMSHANGDIAN_H_H_H

#include "Installed.h"

namespace installed
{
	class MMShangDian : public Installed
	{
	public:
		MMShangDian(HttpSession *http_session, const string &pro_name);
		~MMShangDian();
		
		static int IsMMShangDian(HttpSession *http_session);
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