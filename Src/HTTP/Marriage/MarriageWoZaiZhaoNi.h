#ifndef HTTP_MARRIAGE_WOZAIZHAONI_H_H_H
#define HTTP_MARRIAGE_WOZAIZHAONI_H_H_H

#include "Marriage.h"

namespace marriage
{
	class WoZaiZhaoNiMarriage: public Marriage
	{
	public:
		WoZaiZhaoNiMarriage(HttpSession *http_session, const string &pro_name);
		~WoZaiZhaoNiMarriage();
		static int IsWoZaiZhaoNiMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		//注册
		int WebRegister(const char *packet);
		//登录
		int WebLogin(const char *packet);
		//web修改资料s
		int WebModify(const char *packet);
		//android注册
		int AndroidRegister(const char *packet);
		//android登录
		int AndroidLogin(const char *packet);
		//android修改资料
		int AndroidModify(const char *packet);
		//android定位
		int AndroidLocation(const char *packet);
		enum
		{
			kWebRegister = 1,
			kWebLogin,
			kWebModify,
			kAndroidRegister,
			kAndroidLogin,
			kAndroidModify,
			kAndroidLocation,
		};
	};
}

#endif