#ifndef HTTP_RUNNING_RUNNINSKOUT_H_H_H
#define HTTP_RUNNING_RUNNINSKOUT_H_H_H

#include "Running.h"

namespace running
{
	class Skout : public Running
	{
	public:
		Skout(HttpSession *http_session, const string &pro_name);
		~Skout();
		
		static int IsSkout(HttpSession *http_session);
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