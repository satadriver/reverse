#ifndef HTTP_REALNAME_REALNAMEZHILIANZHAOPIN_H_H_H
#define HTTP_REALNAME_REALNAMEZHILIANZHAOPIN_H_H_H

#include "RealName.h"

namespace realname
{
	class ZhiLianZhaoPin : public RealName
	{
	public:
		ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name);
		~ZhiLianZhaoPin();
		
		static int IsZhiLianZhaoPin(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int RealNameUpdate(const char *packet);
		enum 
		{
			krealname = 1
		};
	};
}

#endif