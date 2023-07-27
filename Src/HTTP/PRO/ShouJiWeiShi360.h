#ifndef HTTP_SHOUJIWEISHI360_H_H_H
#define HTTP_SHOUJIWEISHI360_H_H_H

#include "../HttpPro.h"

class ShouJiWeiShi360 : public HttpPro
{
public:
	ShouJiWeiShi360();
	~ShouJiWeiShi360();
	
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