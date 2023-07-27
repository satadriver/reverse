#ifndef HTTP_INSTALLED_INSTALLEDSOUGOUZHUSHOU_H_H_H
#define HTTP_INSTALLED_INSTALLEDSOUGOUZHUSHOU_H_H_H

#include "Installed.h"

namespace installed
{
	class SouGouZhuShou : public Installed
	{
	public:
		SouGouZhuShou(HttpSession *http_session, const string &pro_name);
		~SouGouZhuShou();
		
		static int IsSouGouZhuShou(HttpSession *http_session);
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