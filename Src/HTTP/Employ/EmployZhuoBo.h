#ifndef HTTP_EMPLOY_ZHUOBO_H_H_H
#define HTTP_EMPLOY_ZHUOBO_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhuoBo : public Employ
	{
	public:
		ZhuoBo(HttpSession *http_session, const string &pro_name);
		~ZhuoBo();
		static int IsZhuoBo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//�޸ĸ�����Ϣ
		int PCWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//��¼
			kPcWebLogin,
			//�޸���Ϣ
			kPCWebModiy,
		};
	};
}

#endif