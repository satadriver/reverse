#ifndef HTTP_EMPLOY_TongCheng58_H_H_H
#define HTTP_EMPLOY_TongCheng58_H_H_H

#include "Employ.h"

namespace employ
{
	class TongCheng58 : public Employ
	{
	public:
		TongCheng58(HttpSession *http_session, const string &pro_name);
		~TongCheng58();
		static int IsTongCheng58(HttpSession *http_session);
		int Process(const char *packet,int action);
		//ȫְ�����½� �޸� ������Ϣ
		int PcWebPaMain(const char *packet);
		//��ְ�����½� �޸� ������Ϣ
		int PcWebPaMain2(const char *packet);
		//android������Ϣ
		int AndroidPaMain(const char *packet);
	private:
		enum
		{
			//ȫְ�����½� �޸� ������Ϣ
			kPcWebPaMain = 1,
		    //��ְ�����½� �޸� ������Ϣ
			kPcWebPaMain2,
			//android������Ϣ
			kAndroidPaMain,
		};
	};
}

#endif