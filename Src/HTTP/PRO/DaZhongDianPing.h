#ifndef HTTP_DAZHONGDIANPING_H_H_H
#define HTTP_DAZHONGDIANPING_H_H_H

#include "../HttpPro.h"
#include "Attached/DeviceInfo.h"

class DaZhongDianPing : public HttpPro
{
public:
	DaZhongDianPing();
	~DaZhongDianPing();
	
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