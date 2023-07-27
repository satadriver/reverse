#ifndef HTTP_INSTALLED_INSTALLEDZHONGXINGSHANGDIAN_H_H_H
#define HTTP_INSTALLED_INSTALLEDZHONGXINGSHAGNDIAN_H_H_H
#include "Installed.h"


namespace installed
{
	class ZhongXingShangDian : public Installed
	{
	public:
		ZhongXingShangDian(HttpSession *http_session, const string &pro_name);
		~ZhongXingShangDian();
		
		static int IsZhongXingShangDian(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		int SoftwareUpdate(const char *packet); 
		enum 
		{
			kSoftwareUpdate = 1,
		};
	};
}

#endif