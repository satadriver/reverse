#ifndef HTTP_RUNNING_RUNNINGVAVST_H_H_H
#define HTTP_RUNNING_RUNNINGVAVST_H_H_H

#include "Running.h"

namespace running
{
	class Avast : public Running
	{
	public:
		Avast(HttpSession *http_session, const string &pro_name);
		~Avast();
		
		static int IsAvast(HttpSession *http_session);
		
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