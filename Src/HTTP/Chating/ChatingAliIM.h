
#ifndef HTTP_CHATING_CHATINGALIIM_H_H_H
#define HTTP_CHATING_CHATINGALIIM_H_H_H

#include "Chating.h"

namespace chating
{
	class AliIM : public Chating
	{
	public:
		AliIM(HttpSession *http_session, const string &pro_name);
		~AliIM();
		static int IsAliIM(HttpSession *http_session);
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