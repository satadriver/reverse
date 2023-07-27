#ifndef HTTP_BLUED_H_H_H
#define HTTP_BLUED_H_H_H

#include "../HttpPro.h"

class Blued : public HttpPro
{
public:
	Blued();
	~Blued();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int DeviceGPSLocation(const char *packet);
	enum 
	{
		kDeviceGPSLocation = 1
	};
};


#endif