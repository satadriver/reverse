
#ifndef HTTP_XINLANGWEIBO_H_H_H
#define HTTP_XINLANGWEIBO_H_H_H

#include "../HttpPro.h"


class XinLangWeiBo : public HttpPro
{
public:
	XinLangWeiBo();
	~XinLangWeiBo();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
protected:
	
private:
	int AddWeiBo(const char *packet);
	int ForwardWeiBo(const char *packet);
	int AddComment(const char *packet);
	
	enum
	{
		kAddWeiBo = 1,
			kForwardWeiBo,
			kAddComment
	};

	
};



#endif