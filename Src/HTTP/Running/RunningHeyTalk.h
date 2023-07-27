#ifndef HTTP_RUNNING_RUNNINHEYTALK_H_H_H
#define HTTP_RUNNING_RUNNINHEYTALK_H_H_H

#include "Running.h"

namespace running
{
	class HeyTalk : public Running
	{
	public:
		HeyTalk(HttpSession *http_session, const string &pro_name);
		~HeyTalk();
		
		static int IsHeyTalk(HttpSession *http_session);
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