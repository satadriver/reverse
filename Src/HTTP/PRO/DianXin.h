#ifndef HTTP_DIANXIN_H_H_H
#define HTTP_DIANXIN_H_H_H

#include "../HttpPro.h"


class DianXin : public HttpPro
{
public:
	DianXin();
	~DianXin();
	
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