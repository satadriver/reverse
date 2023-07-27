#ifndef HTTP_WANDOUJIA_H_H_H
#define HTTP_WANDOUJIA_H_H_H

#include "../HttpPro.h"


class WanDouJia : public HttpPro
{
public:
	WanDouJia();
	~WanDouJia();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int UploadInstalledSoftwareMA(const char *packet);
	enum
	{
		kUploadInstalledSoftwareMA = 1,
	};

};


#endif