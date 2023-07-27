#ifndef HTTP_INSTALLED_INSTALLEDKUAIYONG_H_H_H
#define HTTP_INSTALLED_INSTALLEDKUAIYONG_H_H_H

#include "Installed.h"

namespace installed
{
	class KuaiYong : public Installed
	{
	public:
		KuaiYong(HttpSession *http_session, const string &pro_name);
		~KuaiYong();
		
		static int IsKuaiYong(HttpSession *http_session);
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