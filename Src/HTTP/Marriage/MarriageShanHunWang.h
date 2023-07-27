#ifndef HTTP_MARRIAGE_SHANHUNWANG_H_H_H
#define HTTP_MARRIAGE_SHANHUNWANG_H_H_H

#include "Marriage.h"

namespace marriage
{
	class ShanHunMarriage: public Marriage
	{
	public:
		ShanHunMarriage(HttpSession *http_session, const string &pro_name);
		~ShanHunMarriage();
		static int IsShanHunMarriage(HttpSession *http_session);
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