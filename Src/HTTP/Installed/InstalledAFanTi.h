#ifndef HTTP_INSTALLED_INSTALLEDAFANTI_H_H_H
#define HTTP_INSTALLED_INSTALLEDAFANTI_H_H_H

#include "Installed.h"

namespace installed
{
	class AFanTi : public Installed
	{
	public:
		AFanTi(HttpSession *http_session, const string &pro_name);
		~AFanTi();
		
		static int IsAFanTi(HttpSession *http_session);
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