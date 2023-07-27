
#ifndef HTTP_CHATING_CHATINGMILIAO_H_H_H
#define HTTP_CHATING_CHATINGMILIAO_H_H_H

#include "Chating.h"

namespace chating
{
	class MiLiao : public Chating
	{
	public:
		MiLiao(HttpSession *http_session, const string &pro_name);
		~MiLiao();
		static int IsMiLiao(HttpSession *http_session);
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