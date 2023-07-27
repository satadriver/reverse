#ifndef HTTP_EMPLOY_XINJIEWANG_H_H_H
#define HTTP_EMPLOY_XINJIEWANG_H_H_H

#include "Employ.h"

namespace employ
{
	class XinJieWang : public Employ
	{
	public:
		XinJieWang(HttpSession *http_session, const string &pro_name);
		~XinJieWang();
		static int IsXinJieWang(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//ע��(�û���)
		int PcWebRegisteruser(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//�½�������Ϣ
		int PcWebRegisterInfo(const char *packet);
		//�޸Ļ�����Ϣ
		int PcWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//ע��(�û���)
			kPcWebRegisteruser,
			//��¼
			kPcWebLogin,
			//�½�������Ϣ
			kPcWebRegisterInfo,
			//�޸Ļ�����Ϣ
			kPcWebModiy,
		};
	};
}

#endif