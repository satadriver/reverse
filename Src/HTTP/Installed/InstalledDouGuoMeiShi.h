#ifndef HTTP_INSTALLED_INSTALLEDDOUGUOMEISHI_H_H_H
#define HTTP_INSTALLED_INSTALLEDDOUGUOMEISHI_H_H_H

#include "Installed.h"

namespace installed
{
	class DouGuoMeiShi : public Installed
	{
	public:
		DouGuoMeiShi(HttpSession *http_session, const string &pro_name);
		~DouGuoMeiShi();
		
		static int IsDouGuoMeiShi(HttpSession *http_session);
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