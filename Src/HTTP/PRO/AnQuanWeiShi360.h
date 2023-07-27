#ifndef HTTP_ANQUANWEISHI360_H_H_H
#define HTTP_ANQUANWEISHI360_H_H_H

#include "../HttpPro.h"

class AnQuanWeiShi360 : public HttpPro
{
public:
	AnQuanWeiShi360();
	~AnQuanWeiShi360();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	int ClearVar();
private:
	
	int UploadInstalledSoftwareC(const char *packet);
	enum
	{
		kUploadInstalledSoftwareC = 1,
	};
	
};


#endif