#ifndef HTTP_MBAIDUCOM_H_H_H
#define HTTP_MBAIDUCOM_H_H_H

#include "../HttpPro.h"

class MBaiDuCom : public HttpPro
{
public:
	MBaiDuCom();
	~MBaiDuCom();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int GPSLocation(const char *packet);
	enum 
	{
		kGPSLocation = 1,
	};
};


#endif