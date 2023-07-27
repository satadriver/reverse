
#ifndef HTTP_WANGYIXINWEN_H_H_H
#define HTTP_WANGYIXINWEN_H_H_H

#include "../HttpPro.h"
#include "Attached/DeviceInfo.h"

class WangYiXinWen : public HttpPro
{
public:
	WangYiXinWen();
	~WangYiXinWen();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int AddCommentC(const char *packet);
	int AddCommentM(const char *packet);
	int UploadDeviceInfoM(const char *packet);
	enum 
	{
		kAddCommentC = 1, //2014-11-14
			kAddCommentM = 2, //2014-11-14
			kUploadDeviceInfoM
	};

};


#endif