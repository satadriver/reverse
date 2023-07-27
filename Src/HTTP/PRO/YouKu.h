#ifndef HTTP_YOUKU_H_H_H
#define HTTP_YOUKU_H_H_H

#include "../HttpPro.h"

class YouKu : public HttpPro
{
public:
	YouKu();
	~YouKu();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int UploadDeviceInfoM(const char *packet);
	enum 
	{
		kUploadDeviceInfoM = 1
	};

};


#endif