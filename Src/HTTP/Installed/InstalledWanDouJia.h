#ifndef HTTP_INSTALLED_INSTALLEDWANDOUJIA_H_H_H
#define HTTP_INSTALLED_INSTALLEDWANDOUJIA_H_H_H

#include "Installed.h"

namespace installed
{
	class WanDouJia : public Installed
	{
	public:
		WanDouJia(HttpSession *http_session, const string &pro_name);
		~WanDouJia();
		
		static int IsWanDouJia(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		int SoftwareUpdate1(const char *packet);
		int SoftwareUpdate2(const char *packet);
		int SoftwareUpdate3(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1, //��������
			kSoftwareUpdate2 = 2, //������ѹ������
			kSoftwareUpdate3 = 3, //���� 
		};
	};
}

#endif