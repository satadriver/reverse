#ifndef HTTP_FORUM_FORUMDISCUZ_H_H_H
#define HTTP_FORUM_FORUMDISCUZ_H_H_H


#include "../HttpPro.h"


class Discuz : public HttpPro
{
public:
	Discuz();
	~Discuz();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	int LoginCW(const char *packet);
	int AddPostCW(const char *packet);
	int AddCommentCW(const char *packet);
	
	enum
	{
		kLoginCW = 1,
			kAddPostCW,
			kAddCommentCW
			
	};

};

#endif