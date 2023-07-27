#ifndef HTTP_MARRIAGE_HUATIAN_H_H_H
#define HTTP_MARRIAGE_HUATIAN_H_H_H

#include "Marriage.h"

namespace marriage
{
	class HuaTianMarriage: public Marriage
	{
	public:
		HuaTianMarriage(HttpSession *http_session, const string &pro_name);
		~HuaTianMarriage();
		static int IsHuaTianMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//web修改资料
		int WebModify(const char *packet);
		//android修改资料
		int AndroidModify(const char *packet);
		//android实名认证
		int AndroidRealName(const char *packet);
		//android定位 1
		int AndroidLocation(const char *packet);
		//android定位 2
		int AndroidLocation2(const char *packet);
		enum
		{
			kWebModify = 1,
			kAndroidModify,
			kAndroidRealName,
			kAndroidLocation,
			kAndroidLocation2,
		};
	};
}

#endif