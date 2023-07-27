#ifndef HTTP_INSTALLED_INSTALLEDMEIZUYINGYONGSHANGDIANH_H_H
#define HTTP_INSTALLED_INSTALLEDMEIZUYINGYONGSHANGDIANH_H_H

#include "Installed.h"

namespace installed
{
	class MeiZuYingYongShangDian : public Installed
	{
	public:
		MeiZuYingYongShangDian(HttpSession *http_session, const string &pro_name);
		~MeiZuYingYongShangDian();
		
		static int IsMeiZuYingYongShangDian(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		//�����̵� 1 ����
		int SoftwareUpdate(const char *packet);
		//�����̵� 2 ����
		int SoftwareUpdate1(const char *packet);
		enum 
		{
			kSoftwareUpdate = 1,
			kSoftwareUpdate1 = 2,
		};
	};
}

#endif