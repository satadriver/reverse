#ifndef HTTP_INSTALLED_INSTALLEDSHOUJIZHUSHOU360_H_H_H
#define HTTP_INSTALLED_INSTALLEDSHOUJIZHUSHOU360_H_H_H

#include "Installed.h"

namespace installed
{
	class ShouJiZhuShou360 : public Installed
	{
	public:
		ShouJiZhuShou360(HttpSession *http_session, const string &pro_name);
		~ShouJiZhuShou360();
		
		static int IsShouJiZhuShou360(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadInstalled1(const char *packet);
		int UploadInstalled2(const char *packet);
		int UploadInstalled3(const char *packet);
		enum 
		{	
			kUploadInstalled1 = 1,
			kUploadInstalled2 = 2,
			kUploadInstalled3 = 2
		};
	};
}

#endif