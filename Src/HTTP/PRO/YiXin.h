
#ifndef HTTP_YIXIN_H_H_H
#define HTTP_YIXIN_H_H_H

#include "../HttpPro.h"


class YiXin : public HttpPro
{
public:
	YiXin();
	~YiXin();
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