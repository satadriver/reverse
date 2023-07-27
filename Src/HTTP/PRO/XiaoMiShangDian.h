#ifndef HTTP_XIAOMISHANGDIAN_H_H_H
#define HTTP_XIAOMISHANGDIAN_H_H_H

#include "../HttpPro.h"

class XiaoMiShangDian : public HttpPro
{
public:
	XiaoMiShangDian();
	~XiaoMiShangDian();
	
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