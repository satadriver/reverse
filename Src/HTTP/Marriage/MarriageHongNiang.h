#ifndef HTTP_MARRIAGE_HONGNIANG_H_H_H
#define HTTP_MARRIAGE_HONGNIANG_H_H_H

#include "Marriage.h"

namespace marriage
{
	class HongNiangMarriage: public Marriage
	{
	public:
		HongNiangMarriage(HttpSession *http_session, const string &pro_name);
		~HongNiangMarriage();
		static int IsHongNiangMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		//ע��
		int WebRegister(const char *packet);
		//��¼
		int WebLogin(const char *packet);
		//web�޸�����
		int WebModify(const char *packet);
		//web�޸�����
		int WebModifyM(const char *packet);
		enum
		{
			kWebRegister = 1,
			kWebLogin,
			kWebModify,
			kWebModifyM,
		};
	};
}

#endif