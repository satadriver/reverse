
#ifndef HTTP_CHATING_CHATINGWEIXIN_H_H_H
#define HTTP_CHATING_CHATINGWEIXIN_H_H_H

#include "Chating.h"

namespace chating
{
	class WeiXin : public Chating
	{
	public:
		WeiXin(HttpSession *http_session, const string &pro_name);
		~WeiXin();
		static int IsWeiXin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		int GetDns(const char *packet);
		int Login(const char *packet);
		int UIN(const char *packet);
		int WXID(const char *packet);
		enum
		{
			kGetDns = 1,
			kLogin,
			kUIN,
			kWXID,
		};
		static map<string, time_t>unrepeated_;
	};
}

#endif