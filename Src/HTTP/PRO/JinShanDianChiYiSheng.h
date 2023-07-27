#ifndef HTTP_JINSHANDIANCHIYISHENG_H_H_H
#define HTTP_JINSHANDIANCHIYISHENG_H_H_H

#include "../HttpPro.h"

class JinShanDianChiYiSheng : public HttpPro
{
public:
	JinShanDianChiYiSheng();
	~JinShanDianChiYiSheng();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int UploadDeviceInfoM1(const char *packet);
	enum 
	{
		kUploadDeviceInfoM1 = 1, //2014-10-
	};
	/*
	string imei_;
	string imsi_;
	string os_;
	string version_;
	string model_;*/
};


#endif