
#ifndef HTTP_PAPA_H_H_H
#define HTTP_PAPA_H_H_H

#include "../HttpPro.h"

class PaPa : public HttpPro
{
public:
	PaPa();
	~PaPa();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	int NewTopic(const char *packet);

	enum
	{
		kNewTopic = 1,
	};
	
};


#endif