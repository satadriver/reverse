#ifndef HTTP_RUNNING_RUNNINGCOMODO_H_H_H
#define HTTP_RUNNING_RUNNINGCOMODO_H_H_H

#include "Running.h"

namespace running
{
	class Comodo : public Running
	{
	public:
		Comodo(HttpSession *http_session, const string &pro_name);
		~Comodo();
		
		static int IsComodo(HttpSession *http_session);
		
		int Process(const char *packet,int action);
	private:
		enum
		{
			kOpen = 1
		};
		
		static time_t last_time_;
		
	};
}

#endif