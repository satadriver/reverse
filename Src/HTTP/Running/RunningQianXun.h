#ifndef HTTP_RUNNING_RUNNINGXQIANXUN_H_H_H
#define HTTP_RUNNING_RUNNINGXQIANXUN_H_H_H

#include "Running.h"

namespace running
{
	class QianXun : public Running
	{
	public:
		QianXun(HttpSession *http_session, const string &pro_name);
		~QianXun();
		
		static int IsQianXun(HttpSession *http_session);
		
		int Process(const char *packet,int action);
	private:
		enum
		{
			kOpen = 1
		};
		
		static time_t momo_last_time_;
		
	};

	class CoCo : public Running
	{
	public:
		CoCo(HttpSession *http_session, const string &pro_name);
		~CoCo();
		
		static int IsCoCo(HttpSession *http_session);
		
		int Process(const char *packet,int action);
	private:
		enum
		{
			kOpen = 1
		};
		
		static time_t momo_last_time_;
		
	};
}

#endif