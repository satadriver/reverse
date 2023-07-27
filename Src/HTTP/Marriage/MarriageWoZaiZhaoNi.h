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

		//ע��
		int WebRegister(const char *packet);
		//��¼
		int WebLogin(const char *packet);
		//web�޸�����s
		int WebModify(const char *packet);
		//androidע��
		int AndroidRegister(const char *packet);
		//android��¼
		int AndroidLogin(const char *packet);
		//android�޸�����
		int AndroidModify(const char *packet);
		//android��λ
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