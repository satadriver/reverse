#ifndef HTTP_AIQIYI_H_H_H
#define HTTP_AIQIYI_H_H_H

#include "../HttpPro.h"
#include "Attached/DeviceInfo.h"

class AiQiYi : public HttpPro
{
public:
	AiQiYi();
	~AiQiYi();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int UploadDeviceInfoM(const char *packet);
	enum 
	{
		kUploadDeviceInfoM = 1,
	};

};


#endif