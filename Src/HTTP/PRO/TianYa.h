#ifndef HTTP_TIANYA_H_H_H
#define HTTP_TIANYA_H_H_H

#include "../HttpPro.h"

class TianYa : public HttpPro
{
public:
	TianYa();
	~TianYa();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int AddPostCW(const char *packet);
	int AddPostM(const char *packet);

	int AddCommentCW(const char *packet);
	int AddCommentM(const char *packet);
	enum
	{
		kAddPostCW = 1,
			kAddPostM,
			kAddCommentCW,
			kAddCommentM
	};	
};

#endif