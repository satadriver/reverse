
#ifndef HTTP_JINGDONG_H_H_H
#define HTTP_JINGDONG_H_H_H

#include "../HttpPro.h"

class JingDong : public HttpPro
{
public:
	JingDong();
	~JingDong();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int SubmitOrderCW(const char *packet);
	
	int SubmitOrderM(const char *packet);
	enum
	{
		kSubmitOrderCW = 1,
			kSubmitOrderM,
	};

};



#endif