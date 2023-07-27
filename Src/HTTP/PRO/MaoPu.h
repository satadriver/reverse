
#ifndef HTTP_MAOPU_H_H_H
#define HTTP_MAOPU_H_H_H

#include "../HttpPro.h"

class MaoPu : public HttpPro
{
public:
	MaoPu();
	~MaoPu();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	int LoginCW(const char *packet);
	int LoginM(const char *packet);

	int AddPostCW(const char *packet);
	int AddPostM(const char *packet);

	int AddCommentCW(const char *packet);
	int AddCommentM(const char *packet);
	enum
	{
		kLoginCW = 1,
			kLoginM,
			kAddPostCW,
			kAddPostM,
			kAddCommentCW,
			kAddCommentM
	};
	

};


#endif