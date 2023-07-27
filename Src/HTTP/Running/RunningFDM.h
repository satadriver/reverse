#ifndef HTTP_RUNNING_RUNNINGFDM_H_H_H
#define HTTP_RUNNING_RUNNINGFDM_H_H_H

#include "Running.h"

namespace running
{
	class FDM : public Running
	{
	public:
		FDM(HttpSession *http_session, const string &pro_name);
		~FDM();
		
		static int IsFDM(HttpSession *http_session);
		
		int Process(const char *packet,int action);
		
	};
}

#endif