#ifndef HTTP_MARRIAGE_YIYUANWANG_H_H_H
#define HTTP_MARRIAGE_YIYUANWANG_H_H_H

#include "Marriage.h"

namespace marriage
{
	class YiYuanWangMarriage: public Marriage
	{
	public:
		YiYuanWangMarriage(HttpSession *http_session, const string &pro_name);
		~YiYuanWangMarriage();
		static int IsYiYuanWangMarriage(HttpSession *http_session);
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