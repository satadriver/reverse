
#ifndef HTTP_QQ_H_H_H
#define HTTP_QQ_H_H_H

#include "../HttpPro.h"


class QQ : public HttpPro
{
public:
	QQ();
	~QQ();
	int IsQQ(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int LoginC(const char *packet);
	int SendMessageCW(const char *packet);
	int SendGroupMessageCW(const char *packet);
	int SendGroupVoiceMessageM(const char *packet);
	int UploadDeviceInfo(const char *packet);
	string GetQQ();
	
	enum
	{
		kLoginC = 1,
			kSendMessageCW,
			kSendGroupMessageCW,
			kSendGroupVoiceMessageM,
			kUploadDeviceInfo,
	};
};


#endif