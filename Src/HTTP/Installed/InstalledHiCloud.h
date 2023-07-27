#ifndef HTTP_INSTALLED_INSTALLEDHICLOUD_H_H_H
#define HTTP_INSTALLED_INSTALLEDHICLOUD_H_H_H
#include "Installed.h"


namespace installed
{
	class HiCloud : public Installed
	{
	public:
		HiCloud(HttpSession *http_session, const string &pro_name);
		~HiCloud();
		
		static int IsHiCloud(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		//����ѹѹ����
		int SoftwareUpdate1(const char *packet); 
		//��ѹѹ����
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif