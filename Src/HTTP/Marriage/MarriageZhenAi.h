#ifndef HTTP_MARRIAGE_ZHENAI_H_H_H
#define HTTP_MARRIAGE_ZHENAI_H_H_H

#include "Marriage.h"

namespace marriage
{
	class ZhenAiMarriage: public Marriage
	{
	public:
		ZhenAiMarriage(HttpSession *http_session, const string &pro_name);
		~ZhenAiMarriage();
		static int IsZhenAiMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳע��
		int PcWebRegister(const char *packet);
		//��ҳ��¼
		int PcWebLogin(const char *packet);
		//��ҳ�޸�����
		int PcWebModiy(const char *packet);

		enum
		{
			kPcWebRegister = 1,
			kPcWebLogin,
			kPcWebModiy,
		};
	};
}

#endif