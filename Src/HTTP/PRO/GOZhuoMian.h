#ifndef HTTP_GOZHUOMIAN_H_H_H
#define HTTP_GOZHUOMIAN_H_H_H

#include "../HttpPro.h"


class GOZhuoMian : public HttpPro
{
public:
	GOZhuoMian();
	~GOZhuoMian();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int DeviceGPSLocationM(const char *packet);
	enum 
	{
		kDeviceGPSLocationM = 1,
	};
};


#endif