
#ifndef HTTP_ZHUSHOU91_H_H_H
#define HTTP_ZHUSHOU91_H_H_H

#include "../HttpPro.h"

class ZhuShou91 : public HttpPro
{
public:
	ZhuShou91();
	~ZhuShou91();
	
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:

	int UploadContactsM(const char *packet);
	int UploadInstalledSoftwareMA(const char *packet);
	enum
	{
		kUploadContactsM = 1,
			kUploadInstalledSoftwareMA
	};
	

};

#endif