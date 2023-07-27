#ifndef HTTP_MARRIAGE_YUANLAI_H_H_H
#define HTTP_MARRIAGE_YUANLAI_H_H_H

#include "Marriage.h"

namespace marriage
{
	class YuanLaiMarriage: public Marriage
	{
	public:
		YuanLaiMarriage(HttpSession *http_session, const string &pro_name);
		~YuanLaiMarriage();
		static int IsYuanLaiMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//网页登录
		int PcWebLogin(const char *packet);
		//网页实名认证
		int PcWebCheckName(const char *packet);
		enum
		{
			kPcWebLogin = 1,
			kPcWebCheckName,
		};
	};
}

#endif