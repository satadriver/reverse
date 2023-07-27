#ifndef HTTP_ZHILIANZHAOPIN_H_H_H
#define HTTP_ZHILIANZHAOPIN_H_H_H

#include "../HttpPro.h"

class ZhiLianZhaoPin : public HttpPro
{
public:
	ZhiLianZhaoPin();
	~ZhiLianZhaoPin();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int SaveResumeCW(const char *packet);
	enum 
	{
		kSaveResumeCW = 1
	};
};


#endif