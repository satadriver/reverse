#ifndef HTTP_QIANCHENGWUYOU_H_H_H
#define HTTP_QIANCHENGWUYOU_H_H_H

#include "../HttpPro.h"


class QianChengWuYou : public HttpPro
{
public:
	QianChengWuYou();
	~QianChengWuYou();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int DeviceGPSLocationM(const char *packet);
	int CreateResumeCW(const char *packet);
	enum 
	{
		kDeviceGPSLocationM = 1,
			kCreateResumeCW
	};
};


#endif