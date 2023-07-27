#ifndef HTTP_EMPLOY_YILANYINGCAI_H_H_H
#define HTTP_EMPLOY_YILANYINGCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class YiLanYingCai : public Employ
	{
	public:
		YiLanYingCai(HttpSession *http_session, const string &pro_name);
		~YiLanYingCai();
		static int IsYiLanYingCai(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//�޸ĸ�����Ϣ
		int PcWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//��¼
			kPcWebLogin,
			//�޸Ļ�����Ϣ
			kPcWebModiy,
		};
	};
}

#endif