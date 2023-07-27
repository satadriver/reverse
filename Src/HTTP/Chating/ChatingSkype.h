
#ifndef HTTP_CHATING_CHATINGYIXIN_H_H_H
#define HTTP_CHATING_CHATINGYIXIN_H_H_H

#include "Chating.h"

namespace chating
{
	class YiXin : public Chating
	{
	public:
		YiXin(HttpSession *http_session, const string &pro_name);
		~YiXin();
		static int IsYiXin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int SendVoiceMessageM(const char *packet);
		enum
		{
			kLogin = 1,
			kSendVoiceMessageM,
		};

		string GetAccount(const string &account);
	};
}

#endif