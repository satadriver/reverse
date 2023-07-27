
#ifndef HTTP_WEIXIN_H_H_H
#define HTTP_WEIXIN_H_H_H

#include "../HttpPro.h"

class WeiXin : public HttpPro
{
public:
	WeiXin();
	~WeiXin();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int Login(const char *packet);
	int UIN(const char *packet);
	int WXID(const char *packet);
	enum
	{
		kLogin = 1,
			kUIN,
			kWXID,
	};
	static map<string, time_t>unrepeated_;
};


#endif