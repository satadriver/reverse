#ifndef HTTP_ZHIFUBAOQIANBAO_H_H_H
#define HTTP_ZHIFUBAOQIANBAO_H_H_H

#include "../HttpPro.h"

class ZhiFuBaoQianBao : public HttpPro
{
public:
	ZhiFuBaoQianBao();
	~ZhiFuBaoQianBao();
	
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