#ifndef HTTP_RUNNING_RUNNINTANGO_H_H_H
#define HTTP_RUNNING_RUNNINTANGO_H_H_H

#include "Running.h"

namespace running
{
	class Tango : public Running
	{
	public:
		Tango(HttpSession *http_session, const string &pro_name);
		~Tango();
		
		static int IsTango(HttpSession *http_session);
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