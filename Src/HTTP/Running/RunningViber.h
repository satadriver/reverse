#ifndef HTTP_RUNNING_RUNNINGVIBER_H_H_H
#define HTTP_RUNNING_RUNNINGVIBER_H_H_H

#include "Running.h"

namespace running
{
	class Viber : public Running
	{
	public:
		Viber(HttpSession *http_session, const string &pro_name);
		~Viber();
		
		static int IsViber(HttpSession *http_session);
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