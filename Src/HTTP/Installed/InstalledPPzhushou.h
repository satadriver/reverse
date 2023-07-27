#ifndef HTTP_INSTALLED_INSTALLEDPPZHUSHOU_H_H_H
#define HTTP_INSTALLED_INSTALLEDPPZHUSHOU_H_H_H
/************************************************************************/
/* pp÷˙ ÷                                                          */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class PPzhushou: public Installed
	{
	public:
		PPzhushou(HttpSession *http_session, const string &pro_name);
		~PPzhushou();	
		static int IsPPzhushou(HttpSession *http_session);
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