
#ifndef HTTP_KUAILEBEIYUN_H_H_H
#define HTTP_KUAILEBEIYUN_H_H_H

#include "../HttpPro.h"


class KuaiLeBeiYun : public HttpPro
{
public:
	KuaiLeBeiYun();
	~KuaiLeBeiYun();
	
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