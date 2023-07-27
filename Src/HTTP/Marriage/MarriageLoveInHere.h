#ifndef HTTP_MARRIAGE_LOVEINHERE_H_H_H
#define HTTP_MARRIAGE_LOVEINHERE_H_H_H

#include "Marriage.h"

namespace marriage
{
	class LoveInHereMarriage: public Marriage
	{
	public:
		LoveInHereMarriage(HttpSession *http_session, const string &pro_name);
		~LoveInHereMarriage();
		static int IsLoveInHereMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		//ע��
		int WebRegister(const char *packet);
		//��¼
		int WebLogin(const char *packet);
		//web�޸�����
		int WebModify(const char *packet);
		enum
		{
			kWebRegister = 1,
			kWebLogin,
			kWebModify,
		};
	};
}

#endif