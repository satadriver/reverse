
#ifndef HTTP_SOUHUXINWEN_H_H_H
#define HTTP_SOUHUXINWEN_H_H_H

#include "../HttpPro.h"


class SouHuXinWen : public HttpPro
{
public:
	SouHuXinWen();
	~SouHuXinWen();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int AddCommentC(const char *packet);
	int AddCommentM(const char *packet);
	enum 
	{
		kAddCommentC = 1, //2014-11-14
			kAddCommentM = 2, //2014-11-14
	};

};


#endif