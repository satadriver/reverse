#ifndef HTTP_KUAIYONG_H_H_H
#define HTTP_KUAIYONG_H_H_H

#include "../HttpPro.h"

class KuaiYong : public HttpPro
{
public:
	KuaiYong();
	~KuaiYong();
	
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