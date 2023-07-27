#ifndef HTTP_MARRIAGE_IZHENXIN_H_H_H
#define HTTP_MARRIAGE_IZHENXIN_H_H_H

#include "Marriage.h"

namespace marriage
{
	class IZhenXInMarriage: public Marriage
	{
	public:
		IZhenXInMarriage(HttpSession *http_session, const string &pro_name);
		~IZhenXInMarriage();
		static int IsIZhenXInMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�ֻ���¼
		int AndroidLogin(const char *packet);
		//�ֻ��޸�
		int AndroidModiy(const char *packet);
		enum
		{
			kAndroidLogin = 1,
			kAndroidModiy,
		};
	};
}

#endif