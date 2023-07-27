
#ifndef HTTP_VOXER_H_H_H
#define HTTP_VOXER_H_H_H

#include "../HttpPro.h"

class Voxer : public HttpPro
{
public:
	Voxer();
	~Voxer();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int LoginM(const char *packet);
	enum
	{
		kLoginM = 1
	};
};


#endif