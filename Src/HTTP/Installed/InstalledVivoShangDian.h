#ifndef HTTP_INSTALLED_INSTALLEDVIVOSHANGDIAN_H_H_H
#define HTTP_INSTALLED_INSTALLEDVIVOSHANGDIAN_H_H_H
/************************************************************************/
/* vivo”¶”√…ÃµÍ                                                          */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class VivoShangDian: public Installed
	{
	public:
		VivoShangDian(HttpSession *http_session, const string &pro_name);
		~VivoShangDian();	
		static int IsVivoShangDian(HttpSession *http_session);
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