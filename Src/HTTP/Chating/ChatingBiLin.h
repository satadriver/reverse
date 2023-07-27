
#ifndef HTTP_CHATING_CHATINGBILIN_H_H_H
#define HTTP_CHATING_CHATINGBILIN_H_H_H

#include "Chating.h"

namespace chating
{
	class BiLin : public Chating
	{
	public:
		BiLin(HttpSession *http_session, const string &pro_name);
		~BiLin();
		static int IsBiLin(HttpSession *http_session);
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