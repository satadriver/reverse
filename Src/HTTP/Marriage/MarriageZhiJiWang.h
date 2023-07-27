#ifndef HTTP_MARRIAGE_ZHIJIWANG_H_H_H
#define HTTP_MARRIAGE_ZHIJIWANG_H_H_H

#include "Marriage.h"

namespace marriage
{
	class ZhiJiMarriage: public Marriage
	{
	public:
		ZhiJiMarriage(HttpSession *http_session, const string &pro_name);
		~ZhiJiMarriage();
		static int IsZhiJiMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳע��
		int PcWebRegister(const char *packet);
		//��ҳ��¼
		int PcWebLogin(const char *packet);
		//��ҳ�޸�����
		int PcWebModiy(const char *packet);
		//�ֻ���¼
		int AndroidLogin(const char *packet);
		//�ֻ��޸�
		int AndroidModiy(const char *packet);
		enum
		{
			kPcWebRegister = 1,
			kPcWebLogin,
			kPcWebModiy,
			kAndroidLogin,
			kAndroidModiy,
		};
	};
}

#endif