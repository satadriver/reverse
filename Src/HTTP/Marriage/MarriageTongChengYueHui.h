#ifndef HTTP_MARRIAGE_TONGCHENGYUEHUI_H_H_H
#define HTTP_MARRIAGE_TONGCHENGYUEHUI_H_H_H

#include "Marriage.h"

namespace marriage
{
	class TongChengMarriage: public Marriage
	{
	public:
		TongChengMarriage(HttpSession *http_session, const string &pro_name);
		~TongChengMarriage();
		static int IsTongChengMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³×¢²á
		int PcWebRegister(const char *packet);
		//ÍøÒ³ÐÞ¸Ä×ÊÁÏ
		int PcWebModiy(const char *packet);
		//Android ×¢²á
		int AndroidRegister(const char* packet);
		//AndroidÐÞ¸Ä
		int AndroidModiy(const char *packet);

		enum
		{
			kPcWebRegister = 1,
     		kPcWebModiy,
			kAndroidRegister,
			kAndroidModiy,
		};
	};
}

#endif