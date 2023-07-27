#ifndef HTTP_TENGXUNSHIPIN_H_H_H
#define HTTP_TENGXUNSHIPIN_H_H_H

#include "../HttpPro.h"

class TengXunShiPin : public HttpPro
{
public:
	TengXunShiPin();
	~TengXunShiPin();
	
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