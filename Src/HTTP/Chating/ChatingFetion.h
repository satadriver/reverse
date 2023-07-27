
#ifndef HTTP_CHATING_CHATINGFETION_H_H_H
#define HTTP_CHATING_CHATINGFETION_H_H_H

#include "Chating.h"

namespace chating
{
	class Fetion : public Chating
	{
	public:
		Fetion(HttpSession *http_session, const string &pro_name);
		~Fetion();
		static int IsFetion(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int SendVoiceMessageM(const char *packet);
		enum
		{
			kLogin = 1,
			kSendVoiceMessageM,
		};
	};
}

#endif