
#ifndef HTTP_MILIAO_H_H_H
#define HTTP_MILIAO_H_H_H

#include "../HttpPro.h"


class MiLiao : public HttpPro
{
public:
	MiLiao();
	~MiLiao();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int SendVoiceMessageM(const char *packet);
	enum
	{
		kLogin = 1,
			kSendVoiceMessageM,
	};
	
};


#endif