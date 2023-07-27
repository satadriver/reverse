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
		//网页注册
		int PcWebRegister(const char *packet);
		//网页登录
		int PcWebLogin(const char *packet);
		//网页修改资料
		int PcWebModiy(const char *packet);
		//手机登录
		int AndroidLogin(const char *packet);
		//手机修改
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