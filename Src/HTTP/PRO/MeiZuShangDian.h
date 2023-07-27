
#ifndef HTTP_MEIZUSHANGDIANH_H_H
#define HTTP_MEIZUSHANGDIANH_H_H

#include "../HttpPro.h"

class MeiZuShangDian : public HttpPro
{
public:
	MeiZuShangDian();
	~MeiZuShangDian();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int UploadInstalledSoftwareMA(const char *packet);
	enum 
	{
		kUploadInstalledSoftwareMA = 1
	};

};


#endif