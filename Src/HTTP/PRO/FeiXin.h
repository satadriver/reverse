
#ifndef HTTP_FEIXIN_H_H_H
#define HTTP_FEIXIN_H_H_H

#include "../HttpPro.h"

class FeiXin : public HttpPro
{
public:
	FeiXin();
	~FeiXin();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int SendVoiceMessageM(const char *packet);
	int UploadContactsMA(const char *packet);
	enum
	{
		kLogin = 1,
			kSendVoiceMessageM,
			kUploadContactsMA
	};
/*
	string from_;
	string name_;
	string phone_;*/
};


#endif