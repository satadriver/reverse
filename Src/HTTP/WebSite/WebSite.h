#ifndef HTTP_WEBSITE_H_H_H
#define HTTP_WEBSITE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

namespace website
{
	class WebSite : public HttpPro
	{
	public:
		WebSite(HttpSession *http_session, const string &pro_name);
		virtual ~WebSite();
		static int ProcessWebSite(HttpSession *http_session ,const char *packet);
    //定义变量
	protected:
		string app_;
	};
}

#endif