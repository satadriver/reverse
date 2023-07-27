
#ifndef HTTP_CHATING_CHATINGTALKBOX_H_H_H
#define HTTP_CHATING_CHATINGTALKBOX_H_H_H

#include "Chating.h"

namespace chating
{
	class TalkBox : public Chating
	{
	public:
		TalkBox(HttpSession *http_session, const string &pro_name);
		~TalkBox();
		static int IsTalkBox(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int LoginM(const char *packet);
		enum
		{
			kLoginM = 1,
			kSendMessageM,
			kSendVoiceMessageM,
		};
	};
}

#endif