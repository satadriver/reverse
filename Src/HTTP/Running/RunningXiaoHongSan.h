#ifndef HTTP_RUNNING_RUNNINGXIAOHONGSAN_H_H_H
#define HTTP_RUNNING_RUNNINGXIAOHONGSAN_H_H_H

#include "Running.h"

namespace running
{
	class XiaoHongSan : public Running
	{
	public:
		XiaoHongSan(HttpSession *http_session, const string &pro_name);
		~XiaoHongSan();
		
		static int IsXiaoHongSan(HttpSession *http_session);
		
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