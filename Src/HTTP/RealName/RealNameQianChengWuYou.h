#ifndef HTTP_REALNAME_REALNAMEQIANCHENGWUYOU_H_H_H
#define HTTP_REALNAME_REALNAMEQIANCHENGWUYOU_H_H_H

#include "RealName.h"

namespace realname
{
	class QianChengWuYou : public RealName
	{
	public:
		QianChengWuYou(HttpSession *http_session, const string &pro_name);
		~QianChengWuYou();
		
		static int IsQianChengWuYou(HttpSession *http_session);
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