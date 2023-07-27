#ifndef HTTP_INSTALLED_INSTALLEDOPPOSHANGDIAN_H_H_H
#define HTTP_INSTALLED_INSTALLEDOPPOSHANGDIAN_H_H_H
/************************************************************************/
/* oppo”¶”√…ÃµÍ                                                          */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class OppoShangDian: public Installed
	{
	public:
		OppoShangDian(HttpSession *http_session, const string &pro_name);
		~OppoShangDian();	
		static int IsOppoShangDian(HttpSession *http_session);
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