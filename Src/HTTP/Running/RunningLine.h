#ifndef HTTP_RUNNING_RUNNINLINE_H_H_H
#define HTTP_RUNNING_RUNNINLINE_H_H_H

#include "Running.h"

namespace running
{
	class Line : public Running
	{
	public:
		Line(HttpSession *http_session, const string &pro_name);
		~Line();
		
		static int IsLine(HttpSession *http_session);
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