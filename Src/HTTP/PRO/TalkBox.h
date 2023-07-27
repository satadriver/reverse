
#ifndef HTTP_TALKBOX_H_H_H
#define HTTP_TALKBOX_H_H_H

#include "../HttpPro.h"

class TalkBox : public HttpPro
{
public:
	TalkBox();
	~TalkBox();
	int Is(HttpSession *http_session);
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


#endif