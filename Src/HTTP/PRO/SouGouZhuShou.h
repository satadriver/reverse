#ifndef HTTP_SOUGOUZHUSHOU_H_H_H
#define HTTP_SOUGOUZHUSHOU_H_H_H

#include "../HttpPro.h"

class SouGouZhuShou : public HttpPro
{
public:
	SouGouZhuShou();
	~SouGouZhuShou();
	
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