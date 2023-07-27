#ifndef HTTP_RUNNING_RUNNING_H_H_H
#define HTTP_RUNNING_RUNNING_H_H_H

#include "../HttpPro.h"

namespace running
{
	class Running : public HttpPro
	{
	public:
		Running(HttpSession *http_session, const string &pro_name);
		virtual ~Running();
		
		static int ProcessRunning(HttpSession *http_session ,const char *packet);

	protected:
	//	string software_;
		string version_;


	};
}

#endif