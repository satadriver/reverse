#ifndef HTTP_REALNAME_REALNAMEQITIAN_H_H_H
#define HTTP_REALNAME_REALNAMEQITIAN_H_H_H

#include "RealName.h"

namespace realname
{
	class QiTian : public RealName
	{
	public:
		QiTian(HttpSession *http_session, const string &pro_name);
		~QiTian();
		
		static int IsQiTian(HttpSession *http_session);
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