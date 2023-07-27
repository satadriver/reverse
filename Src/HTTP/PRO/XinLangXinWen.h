
#ifndef HTTP_XINLANGXINWEN_H_H_H
#define HTTP_XINLANGXINWEN_H_H_H

#include "../HttpPro.h"


class XinLangXinWen : public HttpPro
{
public:
	XinLangXinWen();
	~XinLangXinWen();
	
	static int Is(HttpSession *http_session);
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