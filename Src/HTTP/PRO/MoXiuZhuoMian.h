#ifndef HTTP_MOXIUZHUOMIAN_H_H_H
#define HTTP_XIUZHUOMIAN_H_H_H

#include "../HttpPro.h"


class MoXiuZhuoMian : public HttpPro
{
public:
	MoXiuZhuoMian();
	~MoXiuZhuoMian();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int DeviceGPSLocation(const char *packet);
	enum 
	{
		kDeviceGPSLocation = 1,
	};
};


#endif