#ifndef HTTP_MARRIAGE_YLIKE_H_H_H
#define HTTP_MARRIAGE_YLIKE_H_H_H

#include "Marriage.h"

namespace marriage
{
	class YlikeMarriage: public Marriage
	{
	public:
		YlikeMarriage(HttpSession *http_session, const string &pro_name);
		~YlikeMarriage();
		static int IsYlikeMarriage(HttpSession *http_session);
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