
#ifndef HTTP_TENGXUNWEIBO_H_H_H
#define HTTP_TENGXUNWEIBO_H_H_H

#include "../HttpPro.h"


class TengXunWeiBo : public HttpPro
{
public:
	TengXunWeiBo();
	~TengXunWeiBo();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:	
	enum
	{
		kAddWeiBo = 1,
			kForwardWeiBo,
			kAddComment
	};

	
};


#endif